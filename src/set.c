//
// Created by Mikec on 16/10/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/set.h"

//#define TAM_INICIAL 10  // Tamaño inicial del set

// Estructura que define el set de strings
/*typedef struct {
    char **elementos;
    int tamano;
    int capacidad;
} Set;*/

// Funcion para inicializar el set
Set* crear_set() {
    Set *nuevo_set = (Set*) malloc(sizeof(Set));
    nuevo_set->elementos = (char**) malloc(TAM_INICIAL * sizeof(char*));
    nuevo_set->tamano = 0;
    nuevo_set->capacidad = TAM_INICIAL;
    return nuevo_set;
}

// Funcion para liberar la memoria del set
void liberar_set(Set *set) {
    for (int i = 0; i < set->tamano; i++) {
        free(set->elementos[i]);
    }
    free(set->elementos);
    free(set);
}

// Funcion para verificar si un string ya esta en el set
int contiene(Set *set, const char *elemento) {
    for (int i = 0; i < set->tamano; i++) {
        if (strcmp(set->elementos[i], elemento) == 0) {
            return 1;
        }
    }
    return 0;
}

// Funcion para agregar un string al set (si no esta ya presente)
void agregar(Set *set, const char *elemento) {
    if (!contiene(set, elemento)) {
        // Redimensionar si es necesario
        if (set->tamano == set->capacidad) {
            set->capacidad *= 2;
            set->elementos = (char**) realloc(set->elementos, set->capacidad * sizeof(char*));
        }
        // Agregar el nuevo string
        set->elementos[set->tamano] = (char*) malloc((strlen(elemento) + 1) * sizeof(char));
        strcpy(set->elementos[set->tamano], elemento);
        set->tamano++;
    }
}

// Funcion para imprimir el contenido del set
void imprimir_set(Set *set) {
    printf("{ ");
    for (int i = 0; i < set->tamano; i++) {
        printf("\"%s\"", set->elementos[i]);
        if (i < set->tamano - 1) {
            printf(", ");
        }
    }
    printf(" }\n");
}
