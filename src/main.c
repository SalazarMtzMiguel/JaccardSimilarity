#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/set.h"

#define MAX_PALABRAS 100  // Maximo numero de palabras que podemos almacenar
#define MAX_LONGITUD_PALABRA 50  // Maximo tamaño de cada palabra

void quitarEspacios(char *ptr);

void JaccardSimilarityCharacter(char *A, char *B, int k);

char **split_por_espacios(const char *cadena, int *num_palabras);

void imprime_cadenas(char **palabras, int longitud);

void convertir_a_minusculas(char *A);

char *subcadena(const char *cadena, int inicio, int longitud);

char **split_por_k_characters(const char *cadena, int k, int *num_fragmentos);

void liberar_k_grams(char **k_grams, int num_k_grams);

void liberar_cadenas(char **palabras, int num_palabras);

char **crear_k_gramas_palabras(char **palabras, int num_palabras, int k);
void JaccardSimilaritySpacesKGrams(char *A, char *B, int k);

int main(void) {
    char phrase1[] = "Andres is a decent teacher";
    char phrase2[] = "Andres is a decent profesor";
    char phrase3[] = "Andres is a terrible researcher";
    int k = 2;

    printf("%s\n", "Texto A original ");
    printf("%s\n", phrase1);
    printf("%s\n", "Texto B original: ");
    printf("%s\n", phrase2);
    printf("%s\n", "Numero de k-gramas: ");
    printf("%d\n", k);

    convertir_a_minusculas(phrase1);
    convertir_a_minusculas(phrase2);
    convertir_a_minusculas(phrase3);

    JaccardSimilaritySpacesKGrams(phrase1, phrase2, k);
    JaccardSimilarityCharacter(phrase1, phrase2, k);

    return 0;
}

void JaccardSimilaritySpacesKGrams(char *A, char *B, int k) {
    printf("%s\n", "\nVerificando por k-gramas de palabras");

    int num_palabras_A, num_palabras_B;

    // Dividimos las cadenas en palabras
    char **palabrasA = split_por_espacios(A, &num_palabras_A);
    char **palabrasB = split_por_espacios(B, &num_palabras_B);

    // Crear k-gramas de palabras para A
    char **kGramsA = crear_k_gramas_palabras(palabrasA, num_palabras_A, k);
    int num_k_grams_A = num_palabras_A;
    // El numero de k-gramas sera igual al numero de palabras (por comportamiento ciclico)

    // Crear k-gramas de palabras para B
    char **kGramsB = crear_k_gramas_palabras(palabrasB, num_palabras_B, k);
    int num_k_grams_B = num_palabras_B;

    // Crear los sets
    Set *setA = crear_set();
    Set *setB = crear_set();

    // Agregar los k-gramas al setA
    for (int i = 0; i < num_k_grams_A; i++) {
        agregar(setA, kGramsA[i]);
    }

    // Agregar los k-gramas al setB
    for (int i = 0; i < num_k_grams_B; i++) {
        agregar(setB, kGramsB[i]);
    }

    // Calcular la unión e intersección
    Set *setUnion = crear_set();
    Set *setIntersection = crear_set();

    for (int i = 0; i < setA->tamano; ++i) {
        agregar(setUnion, setA->elementos[i]);
        if (contiene(setB, setA->elementos[i])) {
            agregar(setIntersection, setA->elementos[i]);
        }
    }
    for (int i = 0; i < setB->tamano; ++i) {
        agregar(setUnion, setB->elementos[i]);
    }


    printf("%s\n", "Imprimiendo Set A");
    imprimir_set(setA);

    printf("%s\n", "Imprimiendo Set B");
    imprimir_set(setB);

    printf("%s\n", "Imprimiendo Set Union");
    imprimir_set(setUnion);

    printf("%s\n", "Imprimiendo Set intersection");
    imprimir_set(setIntersection);


    printf("%s\n", "El valor de similitud por Jaccard es:");
    printf("%f\n", (float) setIntersection->tamano / (float) setUnion->tamano);
    printf("%s", "Dado que la cardinalidad de la interseccion es: ");
    printf("%d", (int) setIntersection->tamano);
    printf("%s", " y la cardinalidad de la union es: ");
    printf("%d\n", (int) setUnion->tamano);


    // Liberar memoria
    liberar_k_grams(kGramsA, num_k_grams_A);
    liberar_k_grams(kGramsB, num_k_grams_B);
    liberar_set(setA);
    liberar_set(setB);
}

void JaccardSimilarityCharacter(char *A, char *B, int k) {
    int num_k_grams_A, num_k_grams_B;
    quitarEspacios(A);
    quitarEspacios(B);

    printf("%s\n", "\nVerificando por caracteres");

    // Obtener subcadena de A
    char *subA = subcadena(A, 0, k); // Subcadena de los primeros k caracteres de A
    if (subA == NULL) {
        printf("Error al crear la subcadena de A\n");
        return;
    }

    // Crear una nueva cadena para A con el espacio adicional para los k caracteres
    char *newA = malloc(strlen(A) + k + 1); // +1 para el terminador nulo
    if (newA == NULL) {
        printf("Error al reservar memoria para la nueva cadena A.\n");
        free(subA);
        return;
    }
    strcpy(newA, A); // Copiar la cadena original A
    strcat(newA, subA); // Concatenar los primeros k caracteres

    free(subA); // Liberar la memoria de la subcadena

    // Repetir el mismo proceso para B
    char *subB = subcadena(B, 0, k); // Subcadena de los primeros k caracteres de B
    if (subB == NULL) {
        printf("Error al crear la subcadena de B\n");
        free(newA); // Liberar la memoria de newA antes de salir
        return;
    }

    char *newB = malloc(strlen(B) + k + 1); // +1 para el terminador nulo
    if (newB == NULL) {
        printf("Error al reservar memoria para la nueva cadena B.\n");
        free(subB);
        free(newA);
        return;
    }
    strcpy(newB, B); // Copiar la cadena original B
    strcat(newB, subB); // Concatenar los primeros k caracteres

    free(subB); // Liberar la memoria de la subcadena

    // Dividir las nuevas cadenas en k-grams
    char **cadenasA = split_por_k_characters(newA, k, &num_k_grams_A);
    if (cadenasA == NULL) {
        printf("Error al dividir A en k-grams\n");
        free(newA);
        free(newB);
        return;
    }

    char **cadenasB = split_por_k_characters(newB, k, &num_k_grams_B);
    if (cadenasB == NULL) {
        printf("Error al dividir B en k-grams\n");
        liberar_k_grams(cadenasA, num_k_grams_A);
        free(newA);
        free(newB);
        return;
    }

    // Crear los sets
    Set *setA = crear_set();
    Set *setB = crear_set();

    // Agregar los k-grams al setA
    for (int i = 0; i < num_k_grams_A; i++) {
        agregar(setA, cadenasA[i]);
    }
    printf("%s\n", "Imprimiendo Set A");
    imprimir_set(setA);

    // Agregar los k-grams al setB
    for (int i = 0; i < num_k_grams_B; i++) {
        agregar(setB, cadenasB[i]);
    }
    printf("%s\n", "Imprimiendo Set B");
    imprimir_set(setB);
    //

    Set *setUnion = crear_set();
    Set *setIntersection = crear_set();
    for (int i = 0; i < setA->tamano; ++i) {
        agregar(setUnion, setA->elementos[i]);
        if (contiene(setB, setA->elementos[i])) { agregar(setIntersection, setA->elementos[i]); }
    }
    for (int i = 0; i < setB->tamano; ++i) {
        agregar(setUnion, setB->elementos[i]);
    }
    printf("%s\n", "Imprimiendo Set Union");
    imprimir_set(setUnion);

    printf("%s\n", "Imprimiendo Set intersection");
    imprimir_set(setIntersection);

    printf("%s\n", "El valor de similitud por Jaccard es:");
    printf("%f\n", (float) setIntersection->tamano / (float) setUnion->tamano);
    printf("%s", "Dado que la cardinalidad de la interseccion es: ");
    printf("%d", setIntersection->tamano);
    printf("%s", " y la cardinalidad de la union es: ");
    printf("%d", setUnion->tamano);

    //

    // Liberar la memoria usada
    liberar_k_grams(cadenasA, num_k_grams_A);
    liberar_k_grams(cadenasB, num_k_grams_B);
    liberar_set(setA);
    liberar_set(setB);
    liberar_set(setUnion);
    liberar_set(setIntersection);
    free(newA); // Liberar la nueva cadena A
    free(newB); // Liberar la nueva cadena B
}


void convertir_a_minusculas(char *A) {
    for (int i = 0; A[i] != '\0'; i++) {
        A[i] = tolower(A[i]);
    }
}

void quitarEspacios(char *ptr) {
    int i = 0, j = 0;
    while (ptr[i] != '\0') {
        if (ptr[i] != ' ') {
            ptr[j++] = ptr[i];
        }
        i++;
    }
    ptr[j] = '\0';
}

char **split_por_k_characters(const char *cadena, int k, int *num_fragmentos) {
    int longitud_cadena = strlen(cadena);
    *num_fragmentos = longitud_cadena - k + 1; // Calculamos el numero correcto de fragmentos

    // Validamos si hay suficientes caracteres para formar k-grams
    if (*num_fragmentos <= 0) {
        *num_fragmentos = 0;
        return NULL; // No hay suficientes caracteres para formar subcadenas
    }

    // Reservamos espacio para el numero de fragmentos que habra
    char **resultado = malloc(*num_fragmentos * sizeof(char *));
    if (!resultado) {
        printf("Error al reservar memoria para k-grams\n");
        return NULL;
    }

    for (int i = 0; i < *num_fragmentos; i++) {
        // Extraemos la subcadena correspondiente
        resultado[i] = subcadena(cadena, i, k);
    }

    return resultado;
}

char **split_por_espacios(const char *cadena, int *num_palabras) {
    char **resultado = malloc(MAX_PALABRAS * sizeof(char *)); // Arreglo para almacenar punteros a las palabras
    if (!resultado) {
        printf("Error al reservar memoria para split por espacios\n");
        return NULL;
    }

    char *cadena_copia = strdup(cadena); // Hacemos una copia de la cadena original porque strtok la modifica
    if (!cadena_copia) {
        free(resultado);
        printf("Error al duplicar la cadena\n");
        return NULL;
    }

    char *token;
    int contador = 0;

    // Primer token
    token = strtok(cadena_copia, " ");
    while (token != NULL && contador < MAX_PALABRAS) {
        resultado[contador] = strdup(token); // Copiar el token al arreglo
        contador++;
        token = strtok(NULL, " "); // Siguiente token
    }

    *num_palabras = contador; // Guardamos el numero de palabras encontradas

    free(cadena_copia); // Liberamos la copia de la cadena original
    return resultado;
}

void imprime_cadenas(char **palabras, int longitud) {
    // Imprimir el resultado
    printf("Imprimiendo array de cadenas:\n");
    for (int i = 0; i < longitud; i++) {
        printf("%s\n", palabras[i]);
    }
}

char *subcadena(const char *cadena, int inicio, int longitud) {
    // Verificar que el rango sea valido
    if (inicio < 0 || longitud <= 0 || inicio + longitud > strlen(cadena)) {
        return NULL; // Si el rango no es valido, devolver NULL
    }

    // Reservar memoria para la subcadena (+1 para el terminador nulo)
    char *resultado = malloc((longitud + 1) * sizeof(char));
    if (!resultado) {
        printf("Error al reservar memoria para la subcadena\n");
        return NULL;
    }

    // Copiar los caracteres del rango
    strncpy(resultado, cadena + inicio, longitud);
    resultado[longitud] = '\0'; // Agregar terminador nulo

    return resultado;
}

void liberar_k_grams(char **k_grams, int num_k_grams) {
    // Liberar cada k-gram individual
    for (int i = 0; i < num_k_grams; i++) {
        free(k_grams[i]);
    }
    // Liberar el arreglo de punteros
    free(k_grams);
}

void liberar_cadenas(char **palabras, int num_palabras) {
    for (int i = 0; i < num_palabras; i++) {
        free(palabras[i]);
    }
    free(palabras);
}

// Función para crear k-gramas de palabras
char **crear_k_gramas_palabras(char **palabras, int num_palabras, int k) {
    char **kGrams = malloc(num_palabras * sizeof(char *));
    if (!kGrams) {
        printf("Error al reservar memoria para k-gramas de palabras\n");
        return NULL;
    }

    for (int i = 0; i < num_palabras; i++) {
        // Reservar memoria para cada k-grama
        kGrams[i] = malloc(MAX_LONGITUD_PALABRA * k);
        if (!kGrams[i]) {
            printf("Error al reservar memoria para k-grama\n");
            liberar_k_grams(kGrams, i); // Liberar los k-gramas creados hasta ahora
            return NULL;
        }

        // Formar el k-grama ciclico
        strcpy(kGrams[i], palabras[i]); // Primera palabra
        for (int j = 1; j < k; j++) {
            strcat(kGrams[i], " "); // Añadir espacio
            strcat(kGrams[i], palabras[(i + j) % num_palabras]); // Añadir las siguientes palabras ciclicamente
        }
    }

    return kGrams;
}
