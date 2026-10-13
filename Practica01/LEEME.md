# Practica 1: alfabetos y cadenas

## Compilar y ejecutar en Mac

```bash
cd '/Users/axl5136/ESCOM/TEORIA DE LA COMPUTACION /Practica01'
clang -std=c11 -Wall -Wextra -pedantic practica01.c -o practica01
./practica01
```

La carpeta de la materia tiene un espacio al final de su nombre.

## Decisiones de esta version

- Entradas: de 0 a 49 caracteres ASCII (a-z, A-Z, 0-9). Enter solo representa la cadena vacia, mostrada como epsilon.
- Resultados: hasta 999 caracteres. Es un limite elegido para usar arreglos fijos sin memoria dinamica. Si se excede, se informa y no se realiza la operacion.
- Concatenacion: la cadena elegida va primero y la otra despues.
- Potencia positiva: repetir la cadena. Potencia cero: cadena vacia.
- Potencia negativa: se asume que significa repetir la cadena invertida. Ejemplo: ab con exponente -2 produce baba. Confirmar esta convencion con el profesor, porque las diapositivas no la definen.
- Los resultados de concatenacion y potencia se pueden guardar en cualquiera de las dos cadenas para seguir operando.
- Prefijos, sufijos, subcadenas y subsecuencias incluyen la cadena vacia y la cadena completa.
- Subcadenas y subsecuencias se enumeran por posiciones: si hay letras repetidas, pueden aparecer resultados iguales. No se eliminan duplicados.
- Solo se generan subsecuencias para cadenas de hasta 12 caracteres.
- La salida se muestra en pantalla, como permite el enunciado.

## Explicacion por etapas

### 1. Una cadena en C

Una cadena es un arreglo de caracteres que termina con '\0'. Por ejemplo, "hola" ocupa cinco posiciones: h, o, l, a y '\0'. Ese ultimo caracter marca el final y no cuenta en la longitud.

`char cadenas[2][CAPACIDAD]` reserva dos filas. Cada fila guarda una cadena. La primera es `cadenas[0]` y la segunda es `cadenas[1]`: los indices empiezan en cero.

### 2. Longitud

La funcion `longitud` empieza con i = 0. Mientras la posicion actual no contenga '\0', aumenta i. Cuando encuentra el terminador, devuelve el contador con return.

### 3. Entrada

`getchar` lee un caracter. `leerLinea` repite la lectura hasta Enter o el fin de la entrada. Reserva una posicion para '\0' y descarta los caracteres que no caben. Se usa int para poder representar tambien EOF, la marca de fin de entrada.

`leerCadena` comprueba con if que cada caracter sea letra o numero. `leerNumero` transforma los digitos en un entero: al leer 2 y luego 3, calcula 2 * 10 + 3 = 23. Separa el signo y rechaza entradas invalidas.

### 4. Menu

`do ... while` repite el menu. `switch` elige la operacion. `break` termina el caso actual; `continue` vuelve al siguiente recorrido del menu cuando una entrada no permite continuar.

Al elegir cadena 1 o 2, restamos uno para convertir la eleccion en indice 0 o 1.

### 5. Concatenacion y potencia

Concatenar consiste en copiar primero los caracteres de una cadena y despues los de la otra. k indica la siguiente posicion libre del resultado. `k++` aumenta k en uno; en `resultado[k++]` primero se usa su valor actual.

En potencia hay dos ciclos: el externo cuenta repeticiones y el interno recorre los caracteres. Si el exponente es negativo, se lee desde n - 1 hasta 0. Al final siempre se coloca '\0'. Para exponente cero, no hay repeticiones y el resultado queda vacio.

### 6. Prefijos, sufijos y subcadenas

Para abc:

- Prefijos: epsilon, a, ab, abc. Siempre empiezan al principio.
- Sufijos: epsilon, c, bc, abc. Siempre terminan al final.
- Subcadenas: epsilon, a, ab, abc, b, bc, c. Los caracteres deben estar juntos.

Los ciclos controlan el inicio y el final del fragmento, y otro ciclo imprime sus caracteres.

### 7. Subsecuencias

Una subsecuencia permite saltar caracteres, manteniendo el orden. ac es subsecuencia de abc, pero no es subcadena.

Cada posicion tiene dos decisiones: conservarla u omitirla. Por eso hay 2 elevado a n selecciones de posiciones. Para n = 12 son 4096, contando la seleccion vacia.

El programa enumera esas selecciones con numeros. `% 2` obtiene el residuo de dividir entre dos: un 1 indica conservar el caracter actual. `/ 2` descarta esa decision y pasa a la siguiente. Ejemplo: seleccion 5 para abc da residuos 1, 0, 1; conserva a, omite b y conserva c. Imprime ac.

Conviene estudiar esta funcion al final: es la parte menos inmediata de la practica.

## Prueba manual

Introduce ab y 12. Elige concatenacion, cadena 1 y guardar en cadena 1. Se obtiene ab12. Consulta su longitud: debe ser 4. Cambia la primera cadena a ab y calcula potencia -2: debe dar baba, segun la convencion indicada.
