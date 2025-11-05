#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXITO 0
#define ERROR_ARCHIVO 1
#define DERROTA 2
#define ERROR_TOMAR_MEMORIA 3
#define COLA_VACIA 4
#define ESPACIO_DISPONIBLE 5

#define LARGO_MENU 30
#define TAM_NOMBRE 20
#define TAM_RANKING (4 + 4 + TAM_NOMBRE)

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

#define MIN(a, b) ((a)<(b)?(a):(b))

typedef struct{
    char opcion;
    char nombre[TAM_NOMBRE];
    int cantidadPuntos;
    int cantidadMovimientos;
} tPartidaCli;

typedef struct{
    int idJugador;
    char nombre[TAM_NOMBRE];
    int cantidadPuntos;
    int cantidadMovimientos;
    int cantidadPartidas;
} tPartidaSrv;

typedef struct{
    int idJugador;
    char nombre[TAM_NOMBRE];
    int totalPuntos;
} tRanking;

#endif // COMMON_H_INCLUDED
