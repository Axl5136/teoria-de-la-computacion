#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_ENTRADA 50
#define MAX_CADENA 1001
#define MAX_SUBSEQ 12
#define MAX_GUARDADAS 100

char cadenas[MAX_GUARDADAS][MAX_CADENA] = {{0}};
int cantidadCadenas = 0;

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int leerEntero(void) {
    int x;
    while (scanf("%d", &x) != 1) {
        limpiarBuffer();
        printf("Numero no valido: ");
    }
    limpiarBuffer();
    return x;
}

/* Imprime lambda si la cadena esta vacia */
void mostrar(const char *s) {
    if (s[0] == '\0') {
        printf("lambda");
    } else {
        printf("%s", s);
    }
}
void linea(FILE *f, const char *s) {
    printf("  ");
    mostrar(s);
    printf("\n");
    if (f != NULL) {
        if (s[0] == '\0') {
            fprintf(f, "lambda\n");
        } else {
            fprintf(f, "%s\n", s);
        }
    }
}

void copiarParte(char *dest, const char *orig, int inicio, int largo) {
    int i;
    for (i = 0; i < largo; i++) {
        dest[i] = orig[inicio + i];
    }
    dest[largo] = '\0';
}

void invertir(const char *orig, char *dest) {
    int n = (int)strlen(orig);
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = orig[n - 1 - i];
    }
    dest[n] = '\0';
}

void guardarResultado(const char *nueva) {
    if (cantidadCadenas >= MAX_GUARDADAS) {
        printf("Ya no hay espacio para guardar mas cadenas.\n");
        return;
    }

    strcpy(cadenas[cantidadCadenas], nueva);
    printf("W%d = ", cantidadCadenas + 1);
    mostrar(cadenas[cantidadCadenas]);
    printf("\n|w| = %d\n", (int)strlen(cadenas[cantidadCadenas]));
    cantidadCadenas++;
}

int esAlfanumerica(const char *s) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (!isalnum((unsigned char)s[i])) {
            return 0;
        }
    }
    return 1;
}

void leerCadena(char *dest, const char *nombre) {
    char aux[MAX_CADENA];
    int n;

    while (1) {
        printf("%s (letras/numeros, < 50; vacio = lambda): ", nombre);
        if (fgets(aux, sizeof(aux), stdin) == NULL) {
            dest[0] = '\0';
            return;
        }
        n = (int)strlen(aux);
        if (n > 0 && aux[n - 1] == '\n') {
            aux[--n] = '\0';
        }
        if (n >= MAX_ENTRADA) {
            printf("Error: longitud menor a 50.\n");
        } else if (!esAlfanumerica(aux)) {
            printf("Error: solo letras y numeros.\n");
        } else {
            strcpy(dest, aux);
            return;
        }
    }
}

void ingresarCadenas(void) {
    cantidadCadenas = 0;
    leerCadena(cadenas[0], "W1");
    leerCadena(cadenas[1], "W2");
    cantidadCadenas = 2;
}

void mostrarCadenas(void) {
    int i;

    printf("\nCadenas guardadas:\n");
    for (i = 0; i < cantidadCadenas; i++) {
        printf("%d) W%d = ", i + 1, i + 1);
        mostrar(cadenas[i]);
        printf("\n");
    }
}

char *elegirCadena(void) {
    int op;

    do {
        mostrarCadenas();
        printf("Elige una cadena (1-%d): ", cantidadCadenas);
        op = leerEntero();
        if (op < 1 || op > cantidadCadenas) {
            printf("Opcion no valida.\n");
        }
    } while (op < 1 || op > cantidadCadenas);

    return cadenas[op - 1];
}

FILE *preguntarArchivo(void) {
    char resp[8], nombre[80];
    FILE *f;
    int n;

    printf("Guardar en archivo? (s/n): ");
    if (fgets(resp, sizeof(resp), stdin) == NULL || (resp[0] != 's' && resp[0] != 'S')) {
        return NULL;
    }
    printf("Nombre del archivo: ");
    if (fgets(nombre, sizeof(nombre), stdin) == NULL) {
        return NULL;
    }
    n = (int)strlen(nombre);
    if (n > 0 && nombre[n - 1] == '\n') {
        nombre[n - 1] = '\0';
    }
    if (nombre[0] == '\0') {
        strcpy(nombre, "salida.txt");
    }
    f = fopen(nombre, "w");
    if (f == NULL) {
        printf("No se pudo crear el archivo.\n");
    }
    return f;
}

void concatenar(void) {
    char *u, *v, nueva[MAX_CADENA];

    printf("\n--- Concatenacion ---\n");
    printf("Primera cadena (u):\n");
    u = elegirCadena();
    printf("Segunda cadena (v):\n");
    v = elegirCadena();

    if ((int)strlen(u) + (int)strlen(v) >= MAX_CADENA) {
        printf("Resultado demasiado largo.\n");
        return;
    }
    strcpy(nueva, u);
    strcat(nueva, v);
    guardarResultado(nueva);
}

void potencia(void) {
    char *w, base[MAX_CADENA], nueva[MAX_CADENA];
    int k, i, len;

    printf("\n--- Potencia ---\n");
    printf("k>0 concatena, k=0 lambda, k<0 invierte y concatena |k| veces\n");
    w = elegirCadena();
    printf("Exponente k: ");
    k = leerEntero();

    if (k == 0) {
        guardarResultado("");
        return;
    }
    if (k > 0) {
        strcpy(base, w);
    } else {
        invertir(w, base);
        k = -k;
    }
    len = (int)strlen(base);
    if (len > 0 && k > (MAX_CADENA - 1) / len) {
        printf("Resultado demasiado largo.\n");
        return;
    }
    nueva[0] = '\0';
    for (i = 0; i < k; i++) {
        strcat(nueva, base);
    }
    guardarResultado(nueva);
}

void longitud(void) {
    char *w;
    FILE *f;

    printf("\n--- Longitud ---\n");
    w = elegirCadena();
    printf("|w| = %d\n", (int)strlen(w));
    f = preguntarArchivo();
    if (f != NULL) {
        fprintf(f, "Longitud = %d\n", (int)strlen(w));
        fclose(f);
    }
}

void prefijos(void) {
    char *w, aux[MAX_CADENA];
    FILE *f;
    int n, i;

    printf("\n--- Prefijos ---\n");
    w = elegirCadena();
    n = (int)strlen(w);
    f = preguntarArchivo();
    printf("Prefijos:\n");
    linea(f, "");
    for (i = 1; i <= n; i++) {
        copiarParte(aux, w, 0, i);
        linea(f, aux);
    }
    if (f != NULL) {
        fclose(f);
    }
}

void sufijos(void) {
    char *w, aux[MAX_CADENA];
    FILE *f;
    int n, i;

    printf("\n--- Sufijos ---\n");
    w = elegirCadena();
    n = (int)strlen(w);
    f = preguntarArchivo();
    printf("Sufijos:\n");
    linea(f, "");
    for (i = n - 1; i >= 0; i--) {
        copiarParte(aux, w, i, n - i);
        linea(f, aux);
    }
    if (f != NULL) {
        fclose(f);
    }
}

void subcadenas(void) {
    char *w, aux[MAX_CADENA];
    FILE *f;
    int n, i, largo;

    printf("\n--- Subcadenas ---\n");
    w = elegirCadena();
    n = (int)strlen(w);
    f = preguntarArchivo();
    printf("Subcadenas:\n");
    linea(f, "");
    for (i = 0; i < n; i++) {
        for (largo = 1; largo <= n - i; largo++) {
            copiarParte(aux, w, i, largo);
            linea(f, aux);
        }
    }
    if (f != NULL) {
        fclose(f);
    }
}

void generarSubseq(const char *w, int i, char *act, int pos, FILE *f) {
    if (w[i] == '\0') {
        linea(f, act);
        return;
    }
    generarSubseq(w, i + 1, act, pos, f);
    act[pos] = w[i];
    act[pos + 1] = '\0';
    generarSubseq(w, i + 1, act, pos + 1, f); 
    act[pos] = '\0';
}

void subsecuencias(void) {
    char *w, actual[MAX_SUBSEQ + 1];
    FILE *f;

    printf("\n--- Subsecuencias (max 12) ---\n");
    w = elegirCadena();
    if ((int)strlen(w) > MAX_SUBSEQ) {
        printf("Error: longitud maxima 12.\n");
        return;
    }
    f = preguntarArchivo();
    actual[0] = '\0';
    printf("Subsecuencias:\n");
    generarSubseq(w, 0, actual, 0, f);
    if (f != NULL) {
        fclose(f);
    }
}

int main(void) {
    int op;

    printf("Practica 1 - Alfabetos y cadenas\n");
    ingresarCadenas();

    do {
        printf("\n1. Cambiar cadenas\n");
        printf("2. Concatenacion\n");
        printf("3. Potencia\n");
        printf("4. Longitud\n");
        printf("5. Prefijos\n");
        printf("6. Sufijos\n");
        printf("7. Subcadenas\n");
        printf("8. Subsecuencias\n");
        printf("9. Ver cadenas\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        op = leerEntero();

        if (op == 1) {
            ingresarCadenas();
        } else if (op == 2) {
            concatenar();
        } else if (op == 3) {
            potencia();
        } else if (op == 4) {
            longitud();
        } else if (op == 5) {
            prefijos();
        } else if (op == 6) {
            sufijos();
        } else if (op == 7) {
            subcadenas();
        } else if (op == 8) {
            subsecuencias();
        } else if (op == 9) {
            mostrarCadenas();
        } else if (op == 0) {
            printf("Fin.\n");
        } else {
            printf("Opcion no valida.\n");
        }
    } while (op != 0);

    return 0;
}
