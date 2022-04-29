#include "disk.h"

//Arreglo que nos permitira ir marcando
//los bloques logicos
int biteMap[4] = {0,0,0,0};
//Arreglo que nos permitira ir marcando
//los descriptores
int biteMapDescriptors[4] = {0,0,0,0};
//Arreglo que nos indica la cantidad de bytes
//en cada descriptor para facilitar los saltos
int bytesOfDecriptors[4]= {0,0,0,0};
//contador de caracteres para poder 
//seccionar la informacion en bloques
int charsInBlock = 0;//
struct disk *disco;//
int exe = 0;//
char *comando = "";
int opcion = -1;//
char buffer[DISK_SZ];//
char tempBuffer[DISK_SZ];//
int totalChars = 0;//
int lastChar = 0;//
int lastDescriptor = 0;
int totalDescriptors = 0;//
int totalDirs = 0;//
int totalBlocks = 0;//

void read_disk(fd)
{
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
}

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
                int fd = open(dir->d_name, O_RDONLY);
                strcpy(disco->name, dir->d_name);
                printf("Se encontro el disco: %s\n", dir->d_name);
                disco->size = size;
                disco->fd = fd;
                read_disk(disco->fd);
                //printf("contenido: \n%s\n", buffer);
                //Indice del inicio del contenido del disco escrito por el usuario
                int indexBytesDescriptos = indexOf(buffer, "-{")+2;
                printf("indexBytesDescriptos: %d\n", indexBytesDescriptos);

                int indexBytesBlocks = indexOf(buffer, "#{")+2;
                printf("indexBytesBlocks: %d\n", indexBytesBlocks);
                printf("lastChar: %d\n", lastChar);
                //Se calcula el numero de carcateres en el ultimo bloque
                totalChars = lastChar-indexBytesDescriptos-6;
                printf("totalChars: %d\n", totalChars);
                //Se inicializan los mapas de bits
                //Se hacen en dos for distintos por problemas en  la ejecucion
                for (int i = 0; i < 4; i++) {
                    char iesimoDescriptor = buffer[indexBytesDescriptos+i] + '\0';
                    biteMapDescriptors[i] = atoi(&iesimoDescriptor);
                    //printf("biteMapDescriptors[i]: %d\n", biteMapDescriptors[i]);
                }

                for (int i = 0; i < 4; i++) {
                    char iesimoBlock = buffer[indexBytesBlocks+i];
                    biteMap[i] = atoi(&iesimoBlock);
                    //printf("biteMap[i]: %d\n", biteMap[i]);
                }

                //Se inicializan los contadores para los descriptores y los
                //bloques
                totalDescriptors = atoi(&buffer[indexBytesBlocks-5]);
                printf("totalDescriptors: %d\n", totalDescriptors);
                totalBlocks = atoi(&buffer[indexBytesBlocks-3]);
                lastDescriptor = indexBytesDescriptos-2;
                printf("lastDescriptor: %d\n", lastDescriptor);
                int div = totalChars/BLOCK_SZ;
                charsInBlock = totalChars - (div * BLOCK_SZ);
                char nexIsASize = '0';
                char bytesOfDescriptor[4];
                int iAux = 0;
                int iAux2 = 0;
                for (int i = indexBytesBlocks+7; i < indexBytesDescriptos-2; i++)
                {
                    if (nexIsASize == '1')
                    {
                        if(isdigit(buffer[i]) == 1)
                        {
                            bytesOfDescriptor[iAux] = buffer[i];
                            iAux++;
                        }
                    }
                    if (buffer[i] == ']')
                    {
                        nexIsASize = '1';
                    }
                    if (buffer[i] == '>')
                    {
                        bytesOfDecriptors[iAux2] = atoi(bytesOfDescriptor);
                        printf("bytesOfDecriptors[%d]: %d\n", iAux2, bytesOfDecriptors[iAux2]);
                        nexIsASize = '0';
                        iAux = 0;
                        iAux2 ++;
                        memset(bytesOfDescriptor,0,4);
                    }
                }
                return disco;
            }
        }
        closedir(cwd);
        //Se crea el archivo
        printf("No se encontro algun disco\nSe creara uno\n");
        int fd = open("defaul.vhd", O_CREAT | O_RDWR, 777);
        strcpy(disco->name, "defaul.vhd");
        disco->size = size;
        disco->fd = fd;
        disco->descriptors = 0;
        disco->blocks = 0;
        //Se inicializa el mapa de bits con 0 en cada bloque
        sprintf(buffer, "default.vhd#%d#0#0#{0000}>-{0000}", size);
        write(fd, buffer, strlen(buffer));
        lastDescriptor = 29;
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
    //printf("charsInBlock = %d\n", charsInBlock);
    if (length > (BLOCK_SZ - charsInBlock))
    {
        for (int i = 0; i < 4; i ++)
        {
            if (biteMap[i] == 0)
            {
                printf("Se puede escribir pero se actualiza como utilizado el %d bloque\n",i+1);
                biteMap[i] = 1;
                totalBlocks++;
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

int read_block(int disk, int blockn, int showInConsole)
{
    if (blockn > 4)
    {
        printf("el numero de bloque excede el numero de bloques en el disco\n");
        return -1;
    }
    char bufferBlock[DISK_SZ];
    int i = blockn*BLOCK_SZ;
    read_disk(disco->fd);
    int j = (i - BLOCK_SZ);
    lseek(disco->fd, j, SEEK_SET);
    read(disco->fd, bufferBlock, i);
    strcpy(tempBuffer, bufferBlock);
    if (showInConsole == 1) {
        printf("Informacion en el %d bloque\n%s\n",blockn, bufferBlock);
    }
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
    printf("3) Llenar un bloque \n");
    printf("4) Operacion open\n");
    printf("5) Operacion read \n");
    printf("6) Operacion write\n");
    printf("7) Operacion mkdir\n");
    printf("8) Operacion stat\n");
    printf("9) Salir\n");
}

void writeDisk()
{
    int blockN;
    printf("Por favor introduzca la informacion a escribir\n");
    printf("maximo 4096 caracteres\n");
    char imput[DISK_SZ];
    scanf("%s", imput);
    printf("Por favor introduzca el numero del bloque a escribir\n");
    scanf("%d", &blockN);
    write_block(disco->fd, imput, blockN);
}

void readDisk()
{
    int blockN;
    printf("Por favor introduzca el numero del bloque a leer\n");
    scanf("%d", &blockN);
    read_block(disco->fd, blockN, 1);
}

void writeAllBlock()
{
    printf("Por favor introduzca un caracter\n");
    //Este metodo escribe una cadena de 1024 caracteres
    //llenando así todo un bloque
    short int invalidChar = 0;
    char c;
    while((c = getchar()) != '\n' && c != EOF);
    if(scanf("%c", &c) != 1)
        printf("Valor caracter no válido\n");
    else
        w(c);
}

void createDir(char *nameDir)
{
    Descriptor* des = creaCrescriptor(totalDescriptors-totalDescriptors-1, totalBlocks-1, nameDir, 0, '1');
    //int res = insertaBloque(&des, 1)strlen(imput);
    char *string;
    string=(char *)malloc(100*sizeof(char));
    //Obtenemos la representacion en cadena
    //para guardarlo en el disco
    strcpy(string,toStringDescriptor(des));
    strcat(string, "\0");
    totalDirs ++;
    //Se calcula la posicion en el disco
    //correcto para poder guardarlo en el disco
    int salto = lastDescriptor;
    printf("salto: %d\n", salto+7);
    if (totalDescriptors>0)
    {
        memset(buffer,0,BLOCK_SZ);
        lseek(disco->fd, salto+7, SEEK_SET);
        read(disco->fd, buffer, DISK_SZ);
    }
    //Nos posicionamos
    lseek(disco->fd, salto, SEEK_SET);
    //Escribimos en el disco el descriptor
    write(disco->fd, string, strlen(string));
    memset(buffer,0,BLOCK_SZ);
    lseek(disco->fd, 0, SEEK_SET);
    read(disco->fd, buffer, DISK_SZ);
    //Nos posicionamos en el final de los desciptores 
    //para actualizar el mapa de bits de los descriptores
    //que se encuentra en el disco
    int saltoMapaBits = salto+strlen(string);
    lseek(disco->fd, saltoMapaBits, SEEK_SET);
    //Creamos la representacion en cadena del mapa de
    //bits de descriptores
    char bitMapDescriptorsChars[7];
    bitMapDescriptorsChars[0] = '-';
    bitMapDescriptorsChars[1] = '{';
    bitMapDescriptorsChars[2] = biteMapDescriptors[0]+ '0';
    bitMapDescriptorsChars[3] = biteMapDescriptors[1]+ '0';
    bitMapDescriptorsChars[4] = biteMapDescriptors[2]+ '0';
    bitMapDescriptorsChars[5] = biteMapDescriptors[3]+ '0';
    bitMapDescriptorsChars[6] = '}';
    //Lo escribimos en el disco
    write(disco->fd, bitMapDescriptorsChars, 7);
    lastDescriptor += strlen(string);
    //Actualizamos los contadores que nos dicen cuantos 
    //bloques y descriptores tenemos usados
    totalDescriptors+=1;
    printf("Indice del descriptor:\n%d\n", totalDescriptors-totalDirs);
    for (int k = 0; k < 4; k++) {
        printf("bytesOfDecriptors[%d] : %d\n", k, bytesOfDecriptors[k]);
    }
}

void createDescriptor()
{
    //Al tener un tamaño fijo de descriptores
    //debo verificar que no se escriban mas de
    //los permitidos
    if (totalDescriptors-totalDirs < 4) {
        //Se solicitan los parametros necesarios para
        //crear el descriptor
        printf("Primero introduce el nombre\n");
        printf("El nombre no debe tener mas de 100 caracteres\n");
        char name[100];
        scanf("%s", name);
        printf("Por favor introduzca la informacion a escribir\n");
        printf("maximo 4096 caracteres\n");
        char imput[BLOCK_SZ];
        scanf("%s", imput);
        printf("imputSize: %lu\n", strlen(imput));
        printf("lastDescriptor: %d\n", lastDescriptor);
        bytesOfDecriptors[totalDescriptors-totalDirs] = strlen(imput);
        printf("bytesOfDecriptors[totalDescriptors]: %d\n", bytesOfDecriptors[totalDescriptors-totalDirs]);
        //Booleano que nos idicara que ya 
        //se creo el descriptor en el disco
        //y se agrego al mapa de bits de los
        //descriptores
        short writeDescriptor = 0;
        short i = 0;
        //Se crea la estructura de descriptor
        Descriptor* des = creaCrescriptor(totalDescriptors-totalDirs-1, totalBlocks-1, name, strlen(imput), '0');
        //int res = insertaBloque(&des, 1)strlen(imput);
        char *string;
        string=(char *)malloc(100*sizeof(char));
        //Obtenemos la representacion en cadena
        //para guardarlo en el disco
        strcpy(string,toStringDescriptor(des));
        strcat(string, "\0");
        //Se calcula la posicion en el disco
        //correcto para poder guardarlo en el disco
        int salto = lastDescriptor;
        printf("salto: %d\n", salto+7);
        if (totalDescriptors>0)
        {
            memset(buffer,0,BLOCK_SZ);
            lseek(disco->fd, salto+7, SEEK_SET);
            read(disco->fd, buffer, DISK_SZ);
        }
        printf("buffer despues del primer salto: %s\n", buffer);
        //Nos posicionamos
        lseek(disco->fd, salto, SEEK_SET);
        //Escribimos en el disco el descriptor
        write(disco->fd, string, strlen(string));
        memset(buffer,0,BLOCK_SZ);
        lseek(disco->fd, 0, SEEK_SET);
        read(disco->fd, buffer, DISK_SZ);
        printf("buffer despues de escribir: \n%s\n", buffer);
        //Actualizamos el mapa de bits de los
        //descriptores
        while (writeDescriptor == 0)
        {
            if (!biteMapDescriptors[i])
            {
                biteMapDescriptors[i] = 1;
                writeDescriptor = 1;
            } else {
                i++;
            }
        }
        //Nos posicionamos en el final de los desciptores 
        //para actualizar el mapa de bits de los descriptores
        //que se encuentra en el disco
        int saltoMapaBits = salto+strlen(string);
        lseek(disco->fd, saltoMapaBits, SEEK_SET);
        //Creamos la representacion en cadena del mapa de
        //bits de descriptores
        char bitMapDescriptorsChars[7];
        bitMapDescriptorsChars[0] = '-';
        bitMapDescriptorsChars[1] = '{';
        bitMapDescriptorsChars[2] = biteMapDescriptors[0]+ '0';
        bitMapDescriptorsChars[3] = biteMapDescriptors[1]+ '0';
        bitMapDescriptorsChars[4] = biteMapDescriptors[2]+ '0';
        bitMapDescriptorsChars[5] = biteMapDescriptors[3]+ '0';
        bitMapDescriptorsChars[6] = '}';
        //Lo escribimos en el disco
        write(disco->fd, bitMapDescriptorsChars, 7);
        if (totalDescriptors > 0) {
            write(disco->fd, buffer, strlen(buffer));
        }
        lastDescriptor += strlen(string);
        write_block(disco->fd, imput, totalBlocks);
        //Actualizamos los contadores que nos dicen cuantos 
        //bloques y descriptores tenemos usados
        totalDescriptors+=1;
        printf("Indice del descriptor:\n%d\n", totalDescriptors-totalDirs);
        for (int k = 0; k < 4; k++) {
            printf("bytesOfDecriptors[%d] : %d\n", k, bytesOfDecriptors[k]);
        }
    } else {
        printf("Almacenamiento lleno, ya no cabe ningu descriptor\n");
    }
}

void readADescriptor()
{
    printf("Introduce el nombre del descriptor\n");
    char name[100];
    scanf("%s", name);
    int descriptorId = getRealIndexOfDescriptor(name);
    if (descriptorId == -1) {
        printf("No se encontro el descriptor solicitado o es un directorio\n");
        return;
    } else {
        int salto = lastDescriptor;
        if (totalDescriptors-totalDescriptors>0)
        {
            int prevConten = 0;
            for (int i = 0; i < descriptorId-1; i++) {
                prevConten += bytesOfDecriptors[i];
            }
            int realIndexContent = (salto+7) + prevConten;
            memset(buffer,0,BLOCK_SZ);
            lseek(disco->fd, realIndexContent, SEEK_SET);
            read(disco->fd, buffer, bytesOfDecriptors[descriptorId-1]);
            printf("buffer: \n%s\n", buffer);
        }
    }

}

int getRealIndexOfDescriptor(char name[])
{
    memset(buffer,0,BLOCK_SZ);
    lseek(disco->fd, 29, SEEK_SET);
    read(disco->fd, buffer, DISK_SZ);
    int indexOfName = indexOf(buffer, name);
    //Para asegurarnos que no se encontro y por tanto
    //no seguiremos buscando el indice correspondiente
    if (indexOfName == -1) 
    {
        printf("indexOfName == -1\n");
        return -1;
    }
    //Esto para asegurarnos que no econtramos un
    //substring ya que en la representacion en 
    //cadena de un descriptor despues del nombre
    //sigue una coma
    /**if (buffer[indexOfName+1] != ',')
    {
        printf("buffer[indexOfName+1] != ','\n");
        return -1;
    }*/

    //<1,0,Thu Aug  5 19:54:04 2021,Thu Aug  5 19:54:04 2021,501,20,este-no
    int totalCommas = 6;
    for (int i = indexOfName; i > 0; i--)
    {

        if (totalCommas == 0)
        {
            return i-1;
        }
        if (buffer[i] == ',')
        {
            totalCommas--;
        }
    }
    return -1;
}

void saveDisk()
{
    //Se almacena el numero de bloques activos en el archivo
    printf("Se almacena el numero de bloques activos en el archivo\n");
    lseek(disco->fd, 0, SEEK_SET);
    char superBlock[28];
    read(disco->fd, superBlock, 28);
    printf("superBlock\n%s\n", superBlock);
    superBlock[18] = totalDescriptors + '0';
    superBlock[20] = totalBlocks + '0';
    for (int i = 23; i < 26; i++)
    {   
        superBlock[i] = biteMap[i-23] + '0';
    }
        //buffer[26] = '}';
        //buffer[27] = '>';
        //buffer[28] = '#';
        //default.vhd#4096#00000}>#}>#
        //default.vhd#4096#{0000}>#
        //default.vhd#4096#0#0#{0000}>#
    //Aqui esta actualizando la informacion de los bloques para poder
    //almacenarce
    printf("superBlock\n%s\n", superBlock);
    lseek(disco->fd, 0, SEEK_SET);
    write(disco->fd, superBlock, 28);
    totalChars = 0;
    lastChar = 0;
    exe = -1;
}

void assing_option(int option)
{
    switch(option)
    {
        case 1:
            writeDisk();
            break;
        case 2:
            readDisk();
            break;
        case 3:
            writeAllBlock();
            break;
        case 4:
            createDescriptor();
            break;
        case 7:
            printf("Primero Introduce el nombre\n");
            printf("El nombre no debe tener mas de 100 caracteres\n");
            char name[100];
            scanf("%s", name);
            createDir(name);
            break;
        case 8:
            readADescriptor();
            break;
        case 9:
            saveDisk();
            break;
        default:
            printf("opcion invalida, intente nuevamente\n");
            show_menu();
            break;
    }
}

void w(char indice)
{
    char bufferBlock[BLOCK_SZ];
    for (int i = 0; i < BLOCK_SZ-1; i++)
    {
        char temp = indice;
        bufferBlock[i] = temp;
    }
    bufferBlock[BLOCK_SZ-1] = '\0';
    write_block(disco->fd, bufferBlock, 0);
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

char* parseComand(char *comando)
{
    printf("comando: %s\n", comando);
    return strtok(comando, " ");
}

int main(int argc, char const *argv[])
{
    printf("Leyendo el disco\n");
    disco = open_disk(DISK_SZ);
    printf("Bienvenido a la terminal :)\n");
    char *res = "";
    while (exe == 0)
    {
        scanf("%s", comando);
        strcpy(res, parseComand(comando));
        printf("res: \n%s", res);
        //show_menu();
        //scanf("%d", &opcion);
        //assing_option(opcion);
    }
    free(disco);
    return 0;
}