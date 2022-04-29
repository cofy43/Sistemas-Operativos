/**
 * @file programa.c
 * @brief Implementacion del ejercicio 4 de la 
 * Tarea 2 Sistemas Operativos

  @author Martin Felipe Espinal Cruces
  @date 01/07/2022
*/
#include "programa.h"

int main(int argc, char *argv[])
{
    printf("numero de argumentos: %d\n", argc);
    if (argc<3 || argc>4) 
    {
        mensajeError(3);
    } else
    {
        char *bandera = argv[1];
        decide(bandera, argv, argc);
    }
    return 0;
}

float areaCirculo(float radio)
{
    return (float)MPI * pow(radio, 2);
}

float areaTriangulo(float base, float altura)
{
    return (base*altura)/2;
}

float areaCuadrado(float lado)
{
    return pow(lado, 2);
}

float areaRectangulo(float base, float altura)
{
    return base*altura;
}

/**
 * @brief Metodo principal encargado de la verificacion de la bandera y 
 * de los parametros para el calculo de areas
 * @param bandera
 * @param argv. Total de parametros pasados desde la linea de comados
 * @param argc. Contador total de parametros pasados desde la linea de comados
 * */
void decide(char *bandera, char *argv[], int argc)
{
    float area;
    //las condiciones se niegan ya que se devuelve 0 si son iguales y 1 si no lo son
    int short esCircilo = strcmp(bandera, CIRCULO);
    int short esTriangulo = strcmp(bandera, TRIANGULO);
    int short esCuadrado = strcmp(bandera, CUADRADO);
    int short esRectangulo = strcmp(bandera, RECTANGULO);
    if (!esCircilo)
    {
        if (argc == 3)
        {
            float radio = atof(argv[2]);
            area = areaCirculo(radio);
            printf("El Area del circulo es: %f\n", area);
        } else {
            mensajeError(3);
        }
    } else if(!esTriangulo)
    {
        if (argc == 4)
        {
            float base = atof(argv[2]);
            float altura = atof(argv[3]);
            area = areaTriangulo(base, altura);
            printf("El Area del triangulo es: %f\n", area);
        } else {
            mensajeError(2);
        }
    } else if(!esRectangulo && argc == 4)
    {
        if (argc == 4)
        {
            float base = atof(argv[2]);
            float altura = atof(argv[3]);
            area = areaRectangulo(base, altura);
            printf("El Area del rectangulo es: %f\n", area);
        } else {
            mensajeError(2);
        }
    } else if(!esCuadrado)
    {
        if (argc == 3)
        {
            float lado = atof(argv[2]);
            area = areaCuadrado(lado);
            printf("El Area del cuadrado es: %f\n", area);
        } else {
            mensajeError(4);
        }
    } else
    {
        mensajeError(1);
    }
}

/**
 * @brief Metodo encargado de la visualizacion de un error mas especifico 
 * para poder ejecutar de mejor manera el programa.
 * @param id Identificador del error.
 * */
void mensajeError(int id)
{
    switch (id)
    {
    case 1:
        printf("No se reconoce la bandera\n");
        mensajeSintaxis();
        break;
    case 2:
        printf("Se requiere la base y la altura para calcular el area\n");
        mensajeSintaxis();
        break;
    case 3:
        printf("Solo se necesita el valor del radio para calcular el area\n");
        mensajeSintaxis();
        break;
    case 4:
        printf("Solo se necesita el valor de un lado para calcular el area\n");
        mensajeSintaxis();
        break;
    default:
        printf("El numero de argumentos es incorrecto\n");
        mensajeSintaxis();
        break;
    }
}


/**
 * @brief Metodo generico encargado de la visualizacion de la forma
 * correcta de ejecucion del programa
 * */
void mensajeSintaxis()
{
    printf("El programa se debe imprimir como sigue:\n");
    printf("$ ./programa <bandera> <datos>+\n");
}