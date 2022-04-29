#include "arbolHuffman.h"

Nodo *raiz = NULL;

int main(void)
{

}

Tupla *creaTupla(char caracter, int frecuencia)
{
    Tupla *nuevo = NULL;
    nuevo = (Tupla*)malloc(sizeof(Tupla));
    if (nuevo != NULL)
    {
        nuevo->caracter = caracter;
        nuevo->frecuencia = frecuencia;
    }
    return nuevo;
}

Nodo *creaNodo(char caracter, int frecuencia)
{
    Nodo *nuevoNodo = NULL;
    Tupla *nuevaTupla = NULL;
    nuevaTupla = creaTupla(caracter, frecuencia);
    nuevoNodo = (Nodo*)malloc(sizeof(Nodo));

    if (raiz == NULL)
    {
        nuevoNodo->dato = nuevaTupla;
        raiz = nuevoNodo;
    } else
    {
        nuevoNodo->dato = nuevaTupla;
        struct nodo *anterior, *recorrido;
        anterior = NULL;
        recorrido = raiz;
        while (recorrido)
        {
            /* code */
        }
        
    }
}