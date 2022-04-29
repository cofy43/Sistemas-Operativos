#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    char cadena[12]; //Deposito de caracteres
    int leidos;

    //Aoertura de archivos
    int file = open("prueba.txt", O_RDONLY);

    if (file == -1)
    {
        perror("Error al abrir el archivo");
        exit(1);
    }

    //Colocamos el puntero en la posicion  400
    //SEEK_SET -> Inicio del fichero
    //SEEK_CUT -> Posicion relativa actual
    //SEEK_END -> Final del fichero
    lseek(file, 0, SEEK_SET);

    //Leer 10 bytes
    leidos = read(file, cadena, 10);
    close(file);

    cadena[11] = 0;
    printf("Se leyeron %d bytes\nLa cadena es: %s", leidos, cadena);
    

    return 0;
}
