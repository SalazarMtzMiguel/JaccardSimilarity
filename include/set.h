//
// Created by Mikec on 16/10/2024.
//

#ifndef SET_H
#define SET_H
// Definimos la capacidad inicial del set
#define TAM_INICIAL 10

// Estructura que define el set de strings
typedef struct {
    char **elementos;
    int tamano;
    int capacidad;
} Set;

// Declaración de funciones
Set *crear_set();

void liberar_set(Set *set);

int contiene(Set *set, const char *elemento);

void agregar(Set *set, const char *elemento);

void imprimir_set(Set *set);

#endif // SET_H
