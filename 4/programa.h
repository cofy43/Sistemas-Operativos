/**
 * @file programa.h
 * @brief Definicion de Macros, importaciones y funciones para la
 * Tarea 2 Sistemas Operativos

  @author Martin Felipe Espinal Cruces
  @date 01/07/2022
*/
//Paqueteria principal que nos facilita el codigo.
#include<stdio.h>
//Para poder importar funcion atof que hace un parse float
//a los datos enviados desde la terminal con tipo char **.
#include <stdlib.h>
//Para poder utilizar funciones como exponenciacion
//para el calculo de areas.
#include <math.h>
//Para poder utilizar la funcion strcmp la cual facilita la 
//comparacion de cadenas, esto para la identificacion de las banderas.
#include<string.h>
//Definicion de aproximacion de valor de Pi
#ifndef MPI
#define MPI 3.14159265358979323846
#endif
#define CIRCULO "-c"
#define TRIANGULO "-t"
#define CUADRADO "-dc"
#define RECTANGULO "-dr"

void decide(char *bandera, char *argv[], int argc);
float areaCirculo(float radio);
float areaTriangulo(float base, float altura);
float areaCuadrado(float lado);
float areaRectangulo(float base, float altura);
void mensajeError(int id);
void mensajeSintaxis(void);