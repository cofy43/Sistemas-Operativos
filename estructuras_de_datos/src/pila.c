#include "pila.h"
// Todo comienza con el nodo superior
int main(int argc, char const *argv[])
{
    Nodo *superior = NULL;
    agregar(&superior, 1);
    printf("tamanio: %d\n", tamanio(superior));
    agregar(&superior, 2);
    printf("tamanio: %d\n", tamanio(superior));
    agregar(&superior, 3);
    printf("tamanio: %d\n", tamanio(superior));
    agregar(&superior, 4);
    printf("tamanio: %d\n", tamanio(superior));
    agregar(&superior, 5);
    printf("tamanio: %d\n", tamanio(superior));
    toString(superior);
    printf("ultimo: %d\n", ultimo(superior));
    elminarUltimo(&superior);
    toString(superior);
    printf("tamanio: %d\n", tamanio(superior));
    printf("ultimo: %d\n", ultimo(superior));
    toString(superior);
    elminarUltimo(&superior);
    printf("tamanio: %d\n", tamanio(superior));
    printf("ultimo: %d\n", ultimo(superior));
    toString(superior);
    agregar(&superior, 4);
    printf("tamanio: %d\n", tamanio(superior));
    toString(superior);
    return 0;
}

Nodo *creaNodo(int dato) 
{
    Nodo *nuevo = NULL;

    nuevo = (Nodo*)malloc(sizeof(Nodo));
    if (nuevo != NULL)
    {
        nuevo->dato = dato;
        nuevo->siguiente = NULL;
    }
    return nuevo;
}

int agregar(Nodo **superior, int dato)
{
    Nodo *nuevo = NULL;
    nuevo = creaNodo(dato);
    if (nuevo != NULL)
    {
        nuevo->siguiente = *superior;
        *superior = nuevo;
        return 1;
    }
    return 0;
}

int elminarUltimo(Nodo **superior)
{
    if (superior != NULL)
    {
        Nodo *aux = *superior;
        *superior = (*superior)->siguiente;
        free(aux);
        return 1;
    }
    return 0;
}

void toString(Nodo *superior)
{
    Nodo *aux = superior;
    printf("%s\n", "___");
    while (aux != NULL)
    {
        printf("|%d|\n", aux->dato);
        printf("%s\n", "---");
        aux = aux->siguiente;
    }
    printf("\n");
}

int tamanio(Nodo *superior)
{
    Nodo *aux = superior;
    unsigned int size = 0;
    while (aux != NULL)
    {
        size ++;
        aux = aux->siguiente;
    }
    return size;
}

int vacia(Nodo *superior)
{
    return tamanio(superior) == 0;
}

int ultimo(Nodo *superior)
{
    if(superior == NULL) return -1;
    return superior->dato;
}