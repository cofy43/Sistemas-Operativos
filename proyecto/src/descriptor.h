#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "lista.h"
#define _POSIX_SOURCE
#define atoa(x) #x

typedef struct descriptor
{
    int id;
    char* creacion;
    char* modificacion;
    char* name;
    char isDirectory;
    int idUser;
    int idGroup;
    unsigned int bytesOfContent;
    Nodo *bloques;
} Descriptor;


//char *getTime();
int insertaBloque(Descriptor** descriptor, int idBlock);
int eliminaBloque(Descriptor** descriptor, int idBlock);
Descriptor *creaCrescriptor(int lastDescriptorId, int lastBlockId, char *name, int bytesOfContent, char isDirectory);
int actualizaDescriptor(Descriptor** descriptor);
int creaEnlace();
char *toStringDescriptor(Descriptor* descriptor);
void substring(char *s, char sub[], int p, int l);