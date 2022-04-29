#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
/* Not technically required, but needed on some UNIX distributions */
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<ctype.h>
#include "descriptor.h"
#define BLOCK_SZ 1024
#define DISK_SZ 16096

struct disk {
    char name[14];
    int fd;
    int size;
    int descriptors;
    int blocks;
};

struct disk *  open_disk(int size);
int read_block(int disk, int blockn, int showInConsole);
int write_block(int disk, char *buffer, int blockN);
int indexOf(char * theString, char * searched);
void show_menu();
void assing_option(int option);
void w(char opcion);
void crearDescriptor();
void modificarDescriptor();
void leerDescriptor();
//Modularizacion del switch para mantener
//el codigo limpio
void writeDisk();//
void readDisk();//
void writeAllBlock();//
void createDescriptor();//
void modifierDescriptor();
void readADescriptor();//
void createDir(char *nameDir);
void writeADescriptor();
int getRealIndexOfDescriptor(char name[]);//
void saveDisk();//
void substring(char s[], char sub[], int p, int l);
char* parseComand(char *comando);