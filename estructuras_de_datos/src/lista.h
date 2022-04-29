#include <stdio.h>
#include <stdlib.h>

typedef struct nodo
{
    int dato;
    struct nodo *siguiente;
    struct nodo *anterior;
} Nodo;
// Prototipos de funciones
Nodo *creaNodo(int dato);
int insertarInicio(Nodo **cabeza, int dato);
//un * para consulta, ** para modificacion
void toString(Nodo *cabeza);
int insertarFinal(Nodo **cabeza, int dato);
int eliminarNodo(Nodo **cabeza, int dato);
int tamanio(Nodo *cabeza);