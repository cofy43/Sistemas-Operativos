#include "descriptor.h"

char *getTime() 
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char *currentTime = asctime (timeinfo);
    currentTime[strcspn(currentTime, "\n")] = 0;
    return currentTime;
}


int insertaBloque(Descriptor** descriptor, int idBlock)
{
    int restult = insertarFinal(&(*descriptor)->bloques, idBlock);
    return restult;
}

int eliminaBloque(Descriptor** descriptor, int idBlock)
{
    int result = eliminarNodo(&(*descriptor)->bloques, idBlock);
    return result;
}

int cambiaNombre(Descriptor* descriptor, char *newName)
{
    char *lastName =(char *)malloc(60*sizeof(char));
    strcpy(lastName, descriptor->name);
    descriptor->name=(char *)malloc(60*sizeof(char));
    strcpy(descriptor->name, newName);
    return strcmp(lastName, descriptor->name);
}

Descriptor *creaCrescriptor(int lastDescriptorId, int lastBlockId, char *name, int bytesOfContent, char isDirectory)
{
    Descriptor *nuevo = NULL;
    nuevo = (Descriptor*)malloc(sizeof(Descriptor));
    if (nuevo != NULL)
    {
        nuevo->id = lastDescriptorId+1;
        nuevo->idGroup = (int) getgid();
        nuevo->idUser = (int) geteuid();
        nuevo->bloques = creaNodo(lastBlockId+1);
        nuevo->bytesOfContent = bytesOfContent;
        char *time = getTime();
        nuevo->creacion=(char *)malloc(60*sizeof(char));
        nuevo->modificacion=(char *)malloc(60*sizeof(char));
        nuevo->name=(char *)malloc(60*sizeof(char));
        nuevo->isDirectory = isDirectory;
        strcpy(nuevo->creacion,time);
        strcpy(nuevo->modificacion, time);
        strcpy(nuevo->name,name);
        return nuevo;
    }
    printf("Ocurrio un error al crear el descriptor\n");
    return NULL;
}

char *toStringDescriptor(Descriptor* descriptor)
{
    
    char *descriptorString;
    descriptorString=(char *)malloc(500*sizeof(char));
    strcat(descriptorString, "<");
    char id[10];
    sprintf(id, "%d", descriptor->id+1);
    char bytesOfContent[10];
    sprintf(bytesOfContent, "%d", descriptor->bytesOfContent);
    char idUser[10];
    sprintf(idUser, "%d", descriptor->idUser);
    char idGroup[10];
    sprintf(idGroup, "%d", descriptor->idGroup);
    char creation[100];
    sprintf(creation, "%s", descriptor->creacion);
    char modification[100];
    sprintf(modification, "%s", descriptor->modificacion);
    char name[100];
    sprintf(name, "%s", descriptor->name);
    strcat(descriptorString, id);
    strcat(descriptorString, ",");
    strcat(descriptorString, &descriptor->isDirectory);
    strcat(descriptorString, ",");
    strcat(descriptorString, creation);
    strcat(descriptorString, ",");
    strcat(descriptorString, modification);
    strcat(descriptorString, ",");
    strcat(descriptorString, idUser);
    strcat(descriptorString, ",");
    strcat(descriptorString, idGroup);
    strcat(descriptorString, ",");
    strcat(descriptorString, name);
    if (descriptor->isDirectory == '1') {
        strcat(descriptorString, "/");
    }
    strcat(descriptorString, ",");
    char blocksListStrings[50];
    sprintf(blocksListStrings, "%s", toStringList(descriptor->bloques));
    strcat(descriptorString, blocksListStrings);
    strcat(descriptorString, ",");
    strcat(descriptorString, bytesOfContent);
    strcat(descriptorString, ">");
    strcat(descriptorString, "\0");
    return descriptorString;
}