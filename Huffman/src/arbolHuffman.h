#include <stdio.h>
#include <stdlib.h>

typedef struct tupla
{
    char caracter;
    int frecuencia;
} Tupla;

typedef struct  nodo
{
    struct tupla *dato;
    struct nodo *izquierdo;
    struct nodo *derecho;
} Nodo;

Tupla *creaTupla(char cracter, int frecuencia);
Nodo *creaNodo(char caracter, int frecuencia);
void insertar(char cracter, int frecuencia);