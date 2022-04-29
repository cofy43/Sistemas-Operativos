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
    //Creamos la cadena que se va a escribir
    const char* cadena = "Hola mundo";

    //Permisos rw-r--r-x 0645
    //Permisos RW-R--R-- 
    int file = open("prueba.txt", O_CREAT | O_WRONLY, 0664);
    //O_RONLY Abre en modo lectura
    //O_WRONLY Abre en modo escritura
    //O_RDWR Abre en modo escritura/lectura
    //O_CREATED crea un archivo y lo abre
    //O_EXCL Si se usa con O_CREATED si existe el fichero se regresa un error
    //O_TRUNC Abre el archivo y trunca su longitud a 0

    if (file == -1)
    {
        perror("Error al intentar abrir el archivo");
        exit(1);
    }

    //Escritura de la cadena
    write(file, cadena, strlen(cadena));
    close(file);

    return 0;
}
