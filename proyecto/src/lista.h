#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
char *toStringList(Nodo *cabeza);
int insertarFinal(Nodo **cabeza, int dato);
int eliminarNodo(Nodo **cabeza, int dato);
int tamanio(Nodo *cabeza);