#include "cola.h"

int main(int argc, char const *argv[])
{
    Nodo *cola = NULL;
    mete(&cola, 1);
    toString(cola);
    mete(&cola, 2);
    toString(cola);
    mete(&cola, 3);
    toString(cola);
    mete(&cola, 4);
    toString(cola);
    return 0;
}

Nodo *creaNodo(int dato)
{
    Nodo *nuevo = NULL;
    nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo != NULL)
    {
        nuevo->dato = dato;
        nuevo->frente = NULL;
    }
    return nuevo;
}

int mete(Nodo **cola, int dato)
{
    Nodo *nuevo = creaNodo(dato);
    if (nuevo != NULL)
    {
        (*cola)->frente = nuevo;
        *cola = nuevo;
    } else
    {
        *cola = nuevo;
    }
    return 0;
}

int saca(Nodo **cola)
{
    Nodo *aux = *cola, *last = NULL;
    while (aux != NULL)
    {
        last = aux;
        aux = aux->frente;
    }
    last->frente = NULL;
    free(aux);
    return 1;
}

void toString(Nodo *cabeza)
{
    Nodo *aux = cabeza;
    while (aux != NULL)
    {
        printf("|%d|", aux->dato);
        aux = aux->frente;
    }
    printf("\n");
    
}