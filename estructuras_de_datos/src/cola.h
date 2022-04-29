#include <stdio.h>
#include <stdlib.h>

typedef struct nodo
{
    int dato;
    struct nodo *frente;
} Nodo;

// Prototipos de funciones
Nodo *creaNodo(int dato);
int mete(Nodo **cola, int dato);  // push
int saca(Nodo **cola); // pop
void toString(Nodo *cola);
int tamanio(Nodo *cola); // El tamaño de la pila
int vacia(Nodo *cola);  // Indica si la pila está vacía
int ultimo(Nodo *cola);  // El último elemento. Devuelve 0 si no hay elementos