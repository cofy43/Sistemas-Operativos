#include <stdio.h>
#include <stdlib.h>

typedef struct nodo
{
    int dato;
    struct nodo *siguiente;
} Nodo;

// Prototipos de funciones
Nodo *creaNodo(int dato);
int agregar(Nodo **superior, int dato);  // push
int elminarUltimo(Nodo **superior); // pop
void toString(Nodo *superior);
int tamanio(Nodo *superior); // El tamaño de la pila
int vacia(Nodo *superior);  // Indica si la pila está vacía
int ultimo(Nodo *superior);  // El último elemento. Devuelve 0 si no hay elementos