#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXITO 0
#define ERROR_TOMAR_MEMORIA 1
#define COLA_VACIA 2
#define ESPACIO_DISPONIBLE 3

#define PORT 12345
#define BUFFER_SIZE 2048

#define MIN(a, b) ((a)<(b)?(a):(b))

typedef struct{
    char nombre[20];
    int cantidadPuntos;
    int cantidadMovimientos;
} tPartida;

#endif // COMMON_H_INCLUDED
