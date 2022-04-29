#include "disk.h"

//Arreglo que nos permitira ir marcando
//los bloques logicos
int biteMap[4] = {0,0,0,0};
//contador de caracteres para poder 
//seccionar la informacion en bloques
int charsInBlock = 0;
struct disk *disco;
int exe = 0;
int opcion = -1;

struct disk *  open_disk(int size)
{
    DIR * cwd = opendir(".");
    struct dirent * dir;
    int filedesc = 0;
    struct disk *disco;
    disco = malloc(sizeof(struct disk));
    if (cwd)
    {
        while ((dir = readdir(cwd)) != NULL)
        {
            //Saber que tipo de archivo es
            //ver si el nombre cumple con nuestro nombre estander
            //si lo es entonces abre el archivo
            //si no hay nada crea un archivo
            if (dir->d_type == DT_REG && strstr(dir->d_name, "vhd") != NULL)
            {
                printf("Se encontro el disco: %s\n", dir->d_name);
                int fd = open(dir->d_name, O_RDWR);
                disco->size = size;
                disco->fd = fd;
                strcpy(disco->name, "defaul.vhd");
                char buffer[DISK_SZ];
                int totalChars = 0;
                int lastChar = 0;
                int result = 0;
                int fileRead = 0;
                int i = 0;
                //Aqui se realiza la lectura del contenido del disco 
                //para la obtencion del numero de bloques utilizados
                //y obtener el total de caracteres escritos en el disco
                while (fileRead == 0)
                {
                    result = read(fd, &buffer[i], 1);
                    if (result == 0) 
                        fileRead = 1;
                    i++;
                    if (buffer[i] == '\0')
                        lastChar = i;
                }
                //Indice del inicio del contenido del disco escrito por el usuario
                int index = indexOf(buffer, "}>#") + 4;
                //Se calcula el numero de carcateres en el ultimo bloque
                totalChars = lastChar-index;
                int div = totalChars/BLOCK_SZ;
                charsInBlock = totalChars - (div * BLOCK_SZ);
                char biteMapStr[4];
                memcpy(biteMapStr,&buffer[18],4);
                //Se inicializa el mapa de bits con la informacion previamente
                //almacenada
                for (int i = 0; i < 4; i++)
                {
                    char iesimo = biteMapStr[i];
                    biteMap[i] = atoi(&iesimo);
                }
                for (int i = 0; i < 4; i++)
                {
                    char iesimo = biteMapStr[i];
                    printf("biteMap[i] = %d\n", atoi(&iesimo));
                }
                return disco;
            }
        }
        closedir(cwd);
        //Se crea el archivo
        char buffer[DISK_SZ];
        printf("No se encontro algun disco\nSe creara uno\n");
        int fd = open("defaul.vhd", O_CREAT | O_RDWR);
        strcpy(disco->name, "defaul.vhd");
        disco->size = size;
        disco->fd = fd;
        //Se inicializa el mapa de bits con 0 en cada bloque
        sprintf(buffer, "default.vhd#%d#{0000}>#", size);
        write(fd, buffer, strlen(buffer));
        return disco;
    }
    return NULL;
}

int write_block(int disk, char *buffer, int blockN)
{
    //Se limita a solo 4 bloques en el disco
    if (blockN > 4)
    {
        printf("el numero de bloque excede el numero de bloques en el disco\n");
        return -1;
    }
    int length = strlen(buffer);
    //Si no cabe en el bloque actual verificamos si aun hay 
    //otro bloque para almacenarlo
    printf("charsInBlock = %d\n", charsInBlock);
    if (length > (BLOCK_SZ - charsInBlock))
    {
        for (int i = 0; i < 4; i ++)
        {
            if (biteMap[i] == 0)
            {
                printf("Se puede escribir pero se actualiza como utilizado el %d bloque\n",i+1);
                biteMap[i] = 1;
                //Se actualiza el numero de caracteres en el bloque recien utilizado
                charsInBlock = length - (BLOCK_SZ - charsInBlock);
                printf("Se actualiza el numero de caracteres en el bloque recien utilizado\ncaracteres en el nuevo bloque = %d\n",charsInBlock);
                printf("Se ha escrito en el disco con exito\n");
                write(disk, buffer, length);
                return 0;
            }
        }
        //Si llega hasta este punto solo se puede almacenar la informacion que quepa en
        //el ultimo bloque
        write(disk, buffer, (BLOCK_SZ - charsInBlock));
    }
    //Aqui no hay problema porque la informacion
    //Aun cabe en el bloque actual
    write(disk, buffer, length);
    printf("Se ha escrito en el disco con exito\n");
    return 0;
}

int read_block(int disk, int blockn)
{
    if (blockn > 4)
    {
        printf("el numero de bloque excede el numero de bloques en el disco\n");
        return -1;
    }
    char *bufferBlock[DISK_SZ];
    read(disco->fd, bufferBlock, blockn*BLOCK_SZ);
    printf("Informacion en el %d bloque\n%s\n",blockn, *bufferBlock);
    
    return 0;
}

int indexOf(char * theString, char * searched) {
  int position = -1;
  char * result = NULL;

  if (theString && searched) {
    result = strstr(theString, searched);
    if (result) {
      position = result - theString;
    }
  }

  return position;
}

void show_menu()
{
    printf("Por favor seleccione alguna de las siguientes opciones\n");
    printf("1) Escritura de disco\n");
    printf("2) Lectura del disco\n");
    printf("3) Salir\n");
}

void assing_option(int option)
{
    int blockN;
    switch(option)
    {
        case 1:
            printf("Por favor introduzca la informacion a escribir\n");
            printf("maximo 4096 caracteres\n");
            char imput[DISK_SZ];
            scanf("%s", imput);
            printf("Por favor introduzca el numero del bloque a escribir\n");
            scanf("%d", &blockN);
            write_block(disco->fd, imput, blockN);
            break;
        case 2:
            printf("Por favor introduzca el numero del bloque a leer\n");
            scanf("%d", &blockN);
            read_block(disco->fd, blockN);
            break;
        case 3:
            //Se almacena el numero de bloques activos en el archivo
            printf("Se almacena el numero de bloques activos en el archivo\n");
            /*read(disco->fd, buffer, DISK_SZ);
            for (int i = 18; i < 22; i++)
            {
                buffer[i] = biteMap[i-18] + '0';
            }
            //Aqui esta actualizando la informacion de los bloques para poder
            //almacenarce
            printf("buffer: %s\n", buffer);
            write(disco->fd, buffer, 4);*/
            exe = -1;
            break;
        case 4:
            //Este metodo escribe una cadena de 1024 caracteres
            //es temporal, solo lo use para verificar que el aumento
            //en los bloques funcionara
            w();
            break;
        default:
            printf("opcion invalida, intente nuevamente\n");
            show_menu();
            break;
    }
}

void w()
{
    char bufferBlock[BLOCK_SZ];
    for (int i = 0; i < BLOCK_SZ; i++)
    {
        char temp = 'I';
        bufferBlock[i] = temp;
    }
    write_block(disco->fd, bufferBlock, 0);
}

int main(int argc, char const *argv[])
{
    printf("Leyendo el disco\n");
    disco = open_disk(DISK_SZ);
    
    while (exe == 0)
    {
        show_menu();
        scanf("%d", &opcion);
        assing_option(opcion);
    }
    return 0;
}