#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXITO 0
#define ERROR_TOMAR_MEMORIA 1
#define COLA_VACIA 2
#define ESPACIO_DISPONIBLE 3
#define ELEMENTO_REPETIDO 4
#define ELEMENTO_ENCONTRADO 5
#define ELEMENTO_NO_ENCONTRADO 6
#define ERROR_ARCHIVO 7
#define ERROR_INSERCION 8
#define ERROR_CREAR_REGISTRO_INDICE 9
#define ERROR_INSERTAR_EN_ARBOL 10
#define ARCHIVO_NO_EXISTE 11
#define LISTA_VACIA 12
#define ERROR_SOCKET 13

#define ARCH_DATOS "jugadores.dat"
#define ARCH_INDICE "jugadoresIdx.dat"
#define ARCH_PARTIDAS "partidas.dat"

#define PORT 12345
#define BUFFER_SIZE 2048

#define TAM_NOMBRE 20
#define TAM_RANKING (4 + 4 + TAM_NOMBRE)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define ABS_DIFF(a,b) ((a)>(b)?((a)-(b)):((b)-(a)))

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
    int cantidadPartidas;
} tJugador;

typedef struct{
    int idJugador;
    char nombre[TAM_NOMBRE];
    int totalPuntos;
} tRanking;

#endif // COMMON_H_INCLUDED
