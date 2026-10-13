#include <stdio.h>

/*
    Practica 2. Operaciones entre lenguajes.

    Compilar y ejecutar:
        clang -std=c11 -Wall -Wextra -pedantic practica02.c -o practica02
        ./practica02

    Hay tres lenguajes, L1, L2 y L3.
    Cada uno sale de un archivo: las palabras van separadas por espacios.
*/

#define MAX_PALABRAS 400
#define MAX_CARACTERES 200
#define MAX_RESULTADO 8000
#define MAX_RUTA 200

/* Si aqui escribes un archivo, ese lenguaje se carga solo.
   Si lo dejas vacio, el programa pregunta la ruta. */
char rutasFijas[3][MAX_RUTA] = {"", "", ""};

/* lenguajes[0] es L1, lenguajes[1] es L2, lenguajes[2] es L3.
   cantidad[i] dice cuantas palabras tiene ese lenguaje.
   Empiezan vacios. */
char lenguajes[3][MAX_PALABRAS][MAX_CARACTERES] = {{{0}}};
int cantidad[3] = {0, 0, 0};

/* resultado: lo que se muestra al usuario.
   actual: la potencia que ya llevamos construida.
   temporal: la potencia siguiente, antes de copiarla a actual.
   base: el lenguaje que se repite en la potencia. */
char resultado[MAX_RESULTADO][MAX_CARACTERES] = {{0}};
char actual[MAX_RESULTADO][MAX_CARACTERES] = {{0}};
char temporal[MAX_RESULTADO][MAX_CARACTERES] = {{0}};
char base[MAX_PALABRAS][MAX_CARACTERES] = {{0}};

/* ---------- Palabras ---------- */

int longitud(char palabra[]) {
    int i = 0;
    while (palabra[i] != '\0') {
        i++;
    }
    return i;
}

void copiarPalabra(char destino[], char origen[]) {
    int i;
    for (i = 0; origen[i] != '\0'; i++) {
        destino[i] = origen[i];
    }
    destino[i] = '\0';
}

int iguales(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return a[i] == b[i];
}

/* Un lenguaje es un conjunto: la misma palabra no se guarda dos veces. */
int yaEsta(char lenguaje[][MAX_CARACTERES], int n, char palabra[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (iguales(lenguaje[i], palabra)) {
            return 1;
        }
    }
    return 0;
}

int esLetraONumero(unsigned char c) {
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

/* Letras, numeros y letras con acento, como á o ñ. */
int esValida(char palabra[]) {
    int i;
    unsigned char c;

    if (palabra[0] == '\0') {
        return 0;
    }
    for (i = 0; palabra[i] != '\0'; i++) {
        c = (unsigned char)palabra[i];
        if (!esLetraONumero(c) && c < 128) {
            return 0;
        }
    }
    return 1;
}

/* Pega b al final de a. Ejemplo: "casa" y "perro" dan "casaperro". */
int pegarPalabras(char destino[], char a[], char b[]) {
    int i = 0;
    int j = 0;

    while (a[i] != '\0') {
        if (i >= MAX_CARACTERES - 1) {
            return 0;
        }
        destino[i] = a[i];
        i++;
    }
    while (b[j] != '\0') {
        if (i >= MAX_CARACTERES - 1) {
            return 0;
        }
        destino[i] = b[j];
        i++;
        j++;
    }
    destino[i] = '\0';
    return 1;
}

/* Voltea una palabra. Ejemplo: perro -> orrep.
   Una letra con acento ocupa 2 bytes y se mueve completa.
   Ejemplo: leon con acento -> noel con acento. */
void invertir(char destino[], char origen[]) {
    int n;
    int i;
    int k;
    int tam;
    int j;
    unsigned char c;

    n = longitud(origen);
    destino[n] = '\0';
    i = 0;
    k = n;
    while (i < n) {
        c = (unsigned char)origen[i];
        if (!esLetraONumero(c) && i + 1 < n) {
            tam = 2;
        } else {
            tam = 1;
        }
        k = k - tam;
        for (j = 0; j < tam; j++) {
            destino[k + j] = origen[i + j];
        }
        i = i + tam;
    }
}

/* ---------- Lectura de numeros y rutas ---------- */

int leerLinea(char texto[], int capacidad) {
    int c;
    int i = 0;
    int exceso = 0;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (i < capacidad - 1) {
            texto[i] = (char)c;
            i++;
        } else {
            exceso = 1;
        }
    }
    texto[i] = '\0';
    if (c == EOF && i == 0 && exceso == 0) {
        return -1;
    }
    return exceso;
}

/* Convierte lo que escribio el usuario en un entero.
   Si empieza con '-', el numero es negativo. */
int leerNumero(void) {
    char texto[20];
    int estado;
    int i;
    int numero;
    int valido;
    int negativo;

    do {
        estado = leerLinea(texto, 20);
        if (estado == -1) {
            return 0;
        }
        i = 0;
        numero = 0;
        negativo = 0;
        valido = (estado == 0 && texto[0] != '\0');
        if (texto[0] == '-') {
            negativo = 1;
            i = 1;
            if (texto[1] == '\0') {
                valido = 0;
            }
        }
        while (texto[i] != '\0' && valido) {
            if (texto[i] < '0' || texto[i] > '9') {
                valido = 0;
            } else {
                numero = numero * 10 + (texto[i] - '0');
                i++;
            }
        }
        if (!valido) {
            printf("Escribe un numero entero: ");
        }
    } while (!valido);

    if (negativo) {
        return -numero;
    }
    return numero;
}

/* El usuario elige 1, 2 o 3.
   Se resta 1 porque en el arreglo L1 esta en la posicion 0. */
int elegirLenguaje(char mensaje[]) {
    int opcion;

    do {
        printf("%s (1, 2 o 3): ", mensaje);
        opcion = leerNumero();
        if (opcion < 1 || opcion > 3) {
            printf("Elige 1, 2 o 3.\n");
        }
    } while (opcion < 1 || opcion > 3);

    return opcion - 1;
}

/* ---------- Mostrar y guardar ---------- */

void escribirCuenta(int n) {
    if (n == 1) {
        printf("1 palabra");
    } else {
        printf("%d palabras", n);
    }
}

void imprimirPalabra(char palabra[]) {
    if (palabra[0] == '\0') {
        printf("lambda");
    } else {
        printf("%s", palabra);
    }
}

void imprimirLenguaje(char lenguaje[][MAX_CARACTERES], int n) {
    int i;

    if (n == 0) {
        printf("(lenguaje vacio)\n");
        return;
    }
    for (i = 0; i < n; i++) {
        imprimirPalabra(lenguaje[i]);
        printf("\n");
    }
}

void preguntarGuardar(char lenguaje[][MAX_CARACTERES], int n) {
    char respuesta[10];
    char nombre[MAX_RUTA];
    FILE *archivo;
    int i;
    int estado;

    printf("Guardar resultado en archivo? (s/n): ");
    estado = leerLinea(respuesta, 10);
    if (estado == -1 || (respuesta[0] != 's' && respuesta[0] != 'S')) {
        return;
    }

    printf("Nombre del archivo: ");
    estado = leerLinea(nombre, MAX_RUTA);
    if (estado != 0 || nombre[0] == '\0') {
        printf("No se guardo.\n");
        return;
    }

    archivo = fopen(nombre, "w");
    if (archivo == NULL) {
        printf("No se pudo crear el archivo.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        if (lenguaje[i][0] == '\0') {
            fprintf(archivo, "lambda\n");
        } else {
            fprintf(archivo, "%s\n", lenguaje[i]);
        }
    }
    fclose(archivo);
    printf("Guardado en %s\n", nombre);
}

void mostrarResultado(int total) {
    if (total < 0) {
        return;
    }
    printf("\nLenguaje resultante (");
    escribirCuenta(total);
    printf("):\n");
    imprimirLenguaje(resultado, total);
    preguntarGuardar(resultado, total);
}

/* ---------- Cargar los tres archivos ---------- */

void agregarPalabra(int indice, char palabra[]) {
    if (!esValida(palabra)) {
        printf("Se omitio una palabra invalida: %s\n", palabra);
        return;
    }
    if (yaEsta(lenguajes[indice], cantidad[indice], palabra)) {
        return;
    }
    copiarPalabra(lenguajes[indice][cantidad[indice]], palabra);
    cantidad[indice]++;
}

int cargarArchivo(char ruta[], int indice) {
    FILE *archivo;
    char palabra[MAX_CARACTERES];

    archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 0;
    }

    cantidad[indice] = 0;
    /* %s lee una palabra: se detiene en cada espacio. */
    while (fscanf(archivo, "%199s", palabra) == 1) {
        if (cantidad[indice] >= MAX_PALABRAS) {
            printf("Solo se guardaron %d palabras.\n", MAX_PALABRAS);
            break;
        }
        agregarPalabra(indice, palabra);
    }

    fclose(archivo);
    printf("\nLenguaje %d cargado (", indice + 1);
    escribirCuenta(cantidad[indice]);
    printf("):\n");
    imprimirLenguaje(lenguajes[indice], cantidad[indice]);
    return 1;
}

int pedirRuta(int indice) {
    char ruta[MAX_RUTA];
    int estado;

    while (1) {
        printf("Ruta del archivo del lenguaje %d: ", indice + 1);
        estado = leerLinea(ruta, MAX_RUTA);
        if (estado == -1) {
            return 0;
        }
        if (estado == 1 || ruta[0] == '\0') {
            printf("Escribe una ruta valida.\n");
        } else if (cargarArchivo(ruta, indice)) {
            return 1;
        }
    }
}

int cargarTresLenguajes(void) {
    int i;

    printf("Cada archivo contiene las palabras de un lenguaje, separadas por espacios.\n");
    printf("Puedes escribir las rutas aqui, o ponerlas en el arreglo rutasFijas.\n\n");

    for (i = 0; i < 3; i++) {
        if (rutasFijas[i][0] != '\0') {
            if (!cargarArchivo(rutasFijas[i], i)) {
                return 0;
            }
        } else if (!pedirRuta(i)) {
            return 0;
        }
    }
    return 1;
}

/* ---------- Operaciones ---------- */

int copiarLenguaje(char destino[][MAX_CARACTERES],
                   char origen[][MAX_CARACTERES],
                   int n,
                   int maximo) {
    int i;

    if (n > maximo) {
        printf("El resultado supera el limite de %d palabras.\n", maximo);
        return -1;
    }
    for (i = 0; i < n; i++) {
        copiarPalabra(destino[i], origen[i]);
    }
    return n;
}

/* Mete en destino las palabras de origen que todavia no estaban. */
int unirSinRepetir(char destino[][MAX_CARACTERES], int cuantoDestino,
                   char origen[][MAX_CARACTERES], int cuantoOrigen) {
    int i;

    for (i = 0; i < cuantoOrigen; i++) {
        if (!yaEsta(destino, cuantoDestino, origen[i])) {
            if (cuantoDestino >= MAX_RESULTADO) {
                printf("El resultado supera el limite de %d palabras.\n", MAX_RESULTADO);
                return -1;
            }
            copiarPalabra(destino[cuantoDestino], origen[i]);
            cuantoDestino++;
        }
    }
    return cuantoDestino;
}

/* Cada palabra de A pegada con cada palabra de B. */
int concatenar(char a[][MAX_CARACTERES], int na,
               char b[][MAX_CARACTERES], int nb,
               char destino[][MAX_CARACTERES]) {
    int i;
    int j;
    int n = 0;
    char nueva[MAX_CARACTERES];

    if (na == 0 || nb == 0) {
        return 0;
    }
    /* Si no caben todas las combinaciones, no se calcula. */
    if (na > MAX_RESULTADO / nb) {
        printf("El resultado supera el limite de %d palabras.\n", MAX_RESULTADO);
        return -1;
    }

    for (i = 0; i < na; i++) {
        for (j = 0; j < nb; j++) {
            if (!pegarPalabras(nueva, a[i], b[j])) {
                printf("Una palabra del resultado es demasiado larga.\n");
                return -1;
            }
            if (!yaEsta(destino, n, nueva)) {
                copiarPalabra(destino[n], nueva);
                n++;
            }
        }
    }
    return n;
}

/* Voltea cada palabra del lenguaje. */
int voltearLenguaje(char origen[][MAX_CARACTERES], int n,
                    char destino[][MAX_CARACTERES], int maximo) {
    int i;

    if (n > maximo) {
        printf("El resultado supera el limite de %d palabras.\n", maximo);
        return -1;
    }
    for (i = 0; i < n; i++) {
        invertir(destino[i], origen[i]);
    }
    return n;
}

/* Union: primero las palabras de L1 y despues las de L2 que faltaban. */
int unionLenguajes(int i, int j) {
    int n;

    n = copiarLenguaje(resultado, lenguajes[i], cantidad[i], MAX_RESULTADO);
    if (n < 0) {
        return -1;
    }
    return unirSinRepetir(resultado, n, lenguajes[j], cantidad[j]);
}

int concatenacionLenguajes(int i, int j) {
    return concatenar(lenguajes[i], cantidad[i],
                      lenguajes[j], cantidad[j],
                      resultado);
}

/* Reflexion: la potencia -1. Cada palabra queda al reves. */
int reflexionLenguaje(int i) {
    return voltearLenguaje(lenguajes[i], cantidad[i], resultado, MAX_RESULTADO);
}

/* Potencia.
   Exponente 0: solo lambda, la palabra vacia.
   Exponente positivo: se concatena el lenguaje esa cantidad de veces.
   Exponente negativo: primero se voltean las palabras y luego se repiten. */
int potenciaLenguaje(int indice, int exponente) {
    int veces;
    int nBase;
    int nActual;
    int nNuevo;
    int p;

    if (exponente < -5 || exponente > 8) {
        printf("La potencia debe estar entre -5 y 8.\n");
        return -1;
    }
    if (exponente == 0) {
        copiarPalabra(resultado[0], "");
        return 1;
    }

    if (exponente < 0) {
        printf("La potencia negativa invierte cada palabra y despues la repite.\n");
        nBase = voltearLenguaje(lenguajes[indice], cantidad[indice], base, MAX_PALABRAS);
        veces = -exponente;
    } else {
        nBase = copiarLenguaje(base, lenguajes[indice], cantidad[indice], MAX_PALABRAS);
        veces = exponente;
    }
    if (nBase <= 0) {
        return nBase;
    }

    /* actual empieza como L^1. Cada vuelta del for construye la siguiente. */
    nActual = copiarLenguaje(actual, base, nBase, MAX_RESULTADO);
    for (p = 2; p <= veces; p++) {
        nNuevo = concatenar(actual, nActual, base, nBase, temporal);
        if (nNuevo < 0) {
            return -1;
        }
        nActual = copiarLenguaje(actual, temporal, nNuevo, MAX_RESULTADO);
    }
    return copiarLenguaje(resultado, actual, nActual, MAX_RESULTADO);
}

/* Cerradura hasta la potencia 4.
   conLambda = 0 es la positiva: L U L2 U L3 U L4.
   conLambda = 1 es la de Kleene: tambien incluye lambda. */
int cerradura(int indice, int conLambda) {
    int n;
    int nActual;
    int nNuevo;
    int nResultado;
    int p;

    printf("Se calcula hasta la potencia 4.\n");
    n = cantidad[indice];
    nResultado = 0;
    if (conLambda) {
        copiarPalabra(resultado[0], "");
        nResultado = 1;
    }
    if (n == 0) {
        return nResultado;
    }

    nActual = copiarLenguaje(actual, lenguajes[indice], n, MAX_RESULTADO);
    nResultado = unirSinRepetir(resultado, nResultado, actual, nActual);
    if (nResultado < 0) {
        return -1;
    }

    for (p = 2; p <= 4; p++) {
        nNuevo = concatenar(actual, nActual, lenguajes[indice], n, temporal);
        if (nNuevo < 0) {
            return -1;
        }
        nActual = copiarLenguaje(actual, temporal, nNuevo, MAX_RESULTADO);
        nResultado = unirSinRepetir(resultado, nResultado, actual, nActual);
        if (nResultado < 0) {
            return -1;
        }
    }
    return nResultado;
}

int main(void) {
    int opcion;
    int primero;
    int segundo;
    int exponente;
    int total;

    printf("PRACTICA 2 - OPERACIONES ENTRE LENGUAJES\n\n");
    if (!cargarTresLenguajes()) {
        return 0;
    }

    do {
        printf("\nL1: ");
        escribirCuenta(cantidad[0]);
        printf("\nL2: ");
        escribirCuenta(cantidad[1]);
        printf("\nL3: ");
        escribirCuenta(cantidad[2]);
        printf("\n");
        printf("\n1. Union\n");
        printf("2. Concatenacion\n");
        printf("3. Potencia\n");
        printf("4. Cerradura positiva\n");
        printf("5. Cerradura de Kleene\n");
        printf("6. Reflexion\n");
        printf("7. Ver un lenguaje\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        opcion = leerNumero();

        switch (opcion) {
            case 1:
                primero = elegirLenguaje("Primer lenguaje");
                segundo = elegirLenguaje("Segundo lenguaje");
                total = unionLenguajes(primero, segundo);
                mostrarResultado(total);
                break;
            case 2:
                primero = elegirLenguaje("Primer lenguaje");
                segundo = elegirLenguaje("Segundo lenguaje");
                total = concatenacionLenguajes(primero, segundo);
                mostrarResultado(total);
                break;
            case 3:
                primero = elegirLenguaje("Lenguaje");
                printf("Exponente de -5 a 8: ");
                exponente = leerNumero();
                total = potenciaLenguaje(primero, exponente);
                mostrarResultado(total);
                break;
            case 4:
                primero = elegirLenguaje("Lenguaje");
                total = cerradura(primero, 0);
                mostrarResultado(total);
                break;
            case 5:
                primero = elegirLenguaje("Lenguaje");
                total = cerradura(primero, 1);
                mostrarResultado(total);
                break;
            case 6:
                primero = elegirLenguaje("Lenguaje");
                total = reflexionLenguaje(primero);
                mostrarResultado(total);
                break;
            case 7:
                primero = elegirLenguaje("Lenguaje");
                printf("\nL%d (", primero + 1);
                escribirCuenta(cantidad[primero]);
                printf("):\n");
                imprimirLenguaje(lenguajes[primero], cantidad[primero]);
                break;
            case 0:
                printf("Fin.\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 0);

    return 0;
}
