#include <stdio.h>

/* Las entradas tienen hasta 49 caracteres.
   Reservamos mas espacio para reutilizar los resultados. */
#define CAPACIDAD 1000

int longitud(char cadena[]) {
    int i = 0;
    while (cadena[i] != '\0') {
        i++;
    }
    return i;
}

/* Lee una linea completa y descarta lo que no cabe. */
int leerLinea(char texto[], int capacidad) {
    int c, i = 0, exceso = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (i < capacidad - 1) {
            texto[i] = c;
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

int leerCadena(char cadena[]) {
    int estado, valida, i;
    do {
        printf("Escribe una cadena (0 a 49 letras o numeros): ");
        estado = leerLinea(cadena, 50);
        if (estado == -1) {
            return 0;
        }
        valida = (estado == 0);
        for (i = 0; cadena[i] != '\0'; i++) {
            if (!((cadena[i] >= 'a' && cadena[i] <= 'z') ||
                  (cadena[i] >= 'A' && cadena[i] <= 'Z') ||
                  (cadena[i] >= '0' && cadena[i] <= '9'))) {
                valida = 0;
            }
        }
        if (!valida) {
            printf("Entrada invalida. Usa hasta 49 letras sin acentos o numeros, sin espacios.\n");
        }
    } while (!valida);
    return 1;
}

/* Convertimos los digitos con un ciclo para validar toda la entrada.
   Se aceptan enteros entre -9999 y 9999. */
int leerNumero(void) {
    char texto[20];
    int estado, i, signo, numero, valido;
    do {
        estado = leerLinea(texto, 20);
        if (estado == -1) {
            return 0;
        }
        i = 0;
        signo = 1;
        numero = 0;
        valido = (estado == 0);
        if (texto[i] == '-') {
            signo = -1;
            i++;
        }
        if (texto[i] == '\0') {
            valido = 0;
        }
        while (texto[i] != '\0' && valido) {
            if (texto[i] < '0' || texto[i] > '9') {
                valido = 0;
            } else {
                numero = numero * 10 + texto[i] - '0';
                if (numero > 9999) {
                    valido = 0;
                }
            }
            i++;
        }
        if (!valido) {
            printf("Escribe un entero entre -9999 y 9999: ");
        }
    } while (!valido);
    return numero * signo;
}

void mostrar(char cadena[]) {
    if (cadena[0] == '\0') {
        printf("epsilon (cadena vacia)\n");
    } else {
        printf("%s\n", cadena);
    }
}

void copiar(char destino[], char origen[]) {
    int i;
    for (i = 0; origen[i] != '\0'; i++) {
        destino[i] = origen[i];
    }
    destino[i] = '\0';
}

void prefijos(char cadena[]) {
    int i, j, n = longitud(cadena);
    mostrar("");
    for (i = 1; i <= n; i++) {
        for (j = 0; j < i; j++) {
            printf("%c", cadena[j]);
        }
        printf("\n");
    }
}

void sufijos(char cadena[]) {
    int i, j, n = longitud(cadena);
    mostrar("");
    for (i = n - 1; i >= 0; i--) {
        for (j = i; j < n; j++) {
            printf("%c", cadena[j]);
        }
        printf("\n");
    }
}

void subcadenas(char cadena[]) {
    int inicio, fin, j, n = longitud(cadena);
    mostrar("");
    for (inicio = 0; inicio < n; inicio++) {
        for (fin = inicio; fin < n; fin++) {
            for (j = inicio; j <= fin; j++) {
                printf("%c", cadena[j]);
            }
            printf("\n");
        }
    }
}

/* Cada seleccion indica que posiciones conservamos.
   Dividir entre 2 permite revisar una decision por posicion. */
void subsecuencias(char cadena[]) {
    int n = longitud(cadena);
    int total = 1, i, seleccion, decisiones;
    if (n > 12) {
        printf("Esta operacion solo acepta cadenas de hasta 12 caracteres.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        total = total * 2;
    }
    mostrar("");
    for (seleccion = 1; seleccion < total; seleccion++) {
        decisiones = seleccion;
        for (i = 0; i < n; i++) {
            if (decisiones % 2 == 1) {
                printf("%c", cadena[i]);
            }
            decisiones = decisiones / 2;
        }
        printf("\n");
    }
}

int main(void) {
    char cadenas[2][CAPACIDAD];
    char resultado[CAPACIDAD];
    int opcion, elegida, destino, exponente, repeticiones;
    int i, j, k, n, m;

    printf("PRACTICA 1 - ALFABETOS Y CADENAS\nCadena 1\n");
    if (!leerCadena(cadenas[0])) return 0;
    printf("Cadena 2\n");
    if (!leerCadena(cadenas[1])) return 0;

    do {
        printf("\nCadena 1: "); mostrar(cadenas[0]);
        printf("Cadena 2: "); mostrar(cadenas[1]);
        printf("\n1. Longitud\n2. Concatenacion\n3. Potencia\n");
        printf("4. Prefijos\n5. Sufijos\n6. Subcadenas\n7. Subsecuencias\n");
        printf("8. Cambiar una cadena\n0. Salir\nOpcion: ");
        opcion = leerNumero();
        if (opcion == 0) break;
        if (opcion < 1 || opcion > 8) {
            printf("Opcion invalida.\n");
            continue;
        }
        printf("Elige cadena (1 o 2); en concatenacion sera la primera: ");
        elegida = leerNumero();
        if (elegida != 1 && elegida != 2) {
            printf("Cadena invalida.\n");
            continue;
        }
        elegida--;
        n = longitud(cadenas[elegida]);

        switch (opcion) {
            case 1:
                printf("Longitud: %d\n", n);
                break;
            case 2:
                m = longitud(cadenas[1 - elegida]);
                if (n + m >= CAPACIDAD) {
                    printf("El resultado supera el limite de 999 caracteres.\n");
                    continue;
                }
                k = 0;
                for (i = 0; i < n; i++) {
                    resultado[k++] = cadenas[elegida][i];
                }
                for (i = 0; i < m; i++) {
                    resultado[k++] = cadenas[1 - elegida][i];
                }
                resultado[k] = '\0';
                break;
            case 3:
                printf("Exponente (-9999 a 9999): ");
                exponente = leerNumero();
                repeticiones = exponente;
                if (repeticiones < 0) repeticiones = -repeticiones;
                if (n * repeticiones >= CAPACIDAD) {
                    printf("El resultado supera el limite de 999 caracteres.\n");
                    continue;
                }
                k = 0;
                for (i = 0; i < repeticiones; i++) {
                    for (j = 0; j < n; j++) {
                        if (exponente < 0) {
                            resultado[k] = cadenas[elegida][n - 1 - j];
                        } else {
                            resultado[k] = cadenas[elegida][j];
                        }
                        k++;
                    }
                }
                resultado[k] = '\0';
                break;
            case 4: prefijos(cadenas[elegida]); break;
            case 5: sufijos(cadenas[elegida]); break;
            case 6:
                printf("Se muestran las subcadenas por posicion; puede haber repetidas.\n");
                subcadenas(cadenas[elegida]);
                break;
            case 7:
                printf("Se muestran las subsecuencias por posicion; puede haber repetidas.\n");
                subsecuencias(cadenas[elegida]);
                break;
            case 8:
                if (!leerCadena(cadenas[elegida])) return 0;
                break;
        }
        if (opcion == 2 || opcion == 3) {
            printf("Resultado: "); mostrar(resultado);
            printf("Guardar en cadena 1 o 2 para seguir operando (0: no guardar): ");
            destino = leerNumero();
            while (destino < 0 || destino > 2) {
                printf("Elige 0, 1 o 2: ");
                destino = leerNumero();
            }
            if (destino != 0) copiar(cadenas[destino - 1], resultado);
        }
    } while (opcion != 0);
    return 0;
}
