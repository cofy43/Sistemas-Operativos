#include "lista.h"
int main(int argc, char const *argv[])
{
    Nodo *cabeza = NULL;
    insertarInicio(&cabeza, 1);
    printf("tamanio de lista: %u\n" , tamanio(cabeza));
    insertarInicio(&cabeza, 2);
    printf("tamanio de lista: %u\n" , tamanio(cabeza));
    insertarFinal(&cabeza, 3);
    printf("tamanio de lista: %u\n" , tamanio(cabeza));
    insertarFinal(&cabeza, 4);
    printf("tamanio de lista: %u\n" , tamanio(cabeza));
    toString(cabeza);
    eliminarNodo(&cabeza, 1);
    printf("tamanio de lista: %u\n" , tamanio(cabeza));
    toString(cabeza);
    //system("pause>nul");
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
        nuevo->anterior = NULL;
    }
    return nuevo;
}

int insertarInicio(Nodo **cabeza, int dato)
{
    Nodo *nuevo = NULL;
    nuevo = creaNodo(dato);
    if (nuevo != NULL)
    {
        nuevo->siguiente = *cabeza;
        nuevo->anterior = NULL;
        if (*cabeza != NULL)
        {
            (*cabeza)->anterior = nuevo;
        }
        *cabeza = nuevo;
        return 1;
    }
    return 0;
}

char *toStringList(Nodo *cabeza)
{
    char *cadena;
    strcat(cadena, '[');
    Nodo *aux = cabeza;
    while (aux != NULL)
    {
        strcat(cadena, aux->dato);
        strcat(cadena, ',');
        //printf("|%d|", aux->dato);
        aux = aux->siguiente;
    }
    //printf("\n");
    strcat(cadena, ']');
    return cadena;
}

int insertarFinal(Nodo **cabeza, int dato)
{
    Nodo *nuevo = NULL, *aux = *cabeza;
    nuevo = creaNodo(dato);
    if (nuevo != NULL)
    {
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }
        nuevo->anterior = aux;
        aux->siguiente = nuevo;
        return 1;
    }
    return 0;
}

int eliminarNodo(Nodo **cabeza, int dato)
{
    Nodo *actual = *cabeza, *anterior = NULL, *siguiente = NULL;
    while (actual != NULL)
    {
        if (actual->dato == dato)
        {
            //si el nodo actual es la cabeza
            if (actual == *cabeza)
            {
                *cabeza = actual->siguiente;
                if (actual->siguiente != NULL)
                {
                    actual->siguiente->anterior = NULL;
                }
            }
            //Si el nodo es la cola
            else if (actual->siguiente == NULL)
            {
                anterior = actual->anterior;
                actual->anterior = NULL;
                anterior->siguiente = NULL;
            }
            //Si el nodo es un nodo intermedio
            else
            {
                anterior = actual->anterior;
                actual->anterior = NULL;
                siguiente = actual->siguiente;
                actual->siguiente = NULL;
                anterior->siguiente = siguiente;
                siguiente->anterior = anterior;
            }
            //liberamos memoria del nodo actual
            free(actual);
            return 1;
        }
        actual = actual->siguiente;
    }
    return 0;
}

int tamanio(Nodo *cabeza)
{
    unsigned int size = 0;
    Nodo *aux = cabeza;
    while (aux != NULL)
    {
        size ++;
        aux = aux->siguiente;
    }
    return size;
}