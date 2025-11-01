#ifndef LABERINTO_H_INCLUDED
#define LABERINTO_H_INCLUDED

#include "common.h"

#include "cola.h"

// ----------- Estructuras --------------
typedef struct {
    int i, j;
} Pos;

typedef struct {
    int id;
    Pos posicion;
    int estaVivo;
    char debajo;
} Fantasma;

typedef struct {
    int id;
    char proxMov;
} MovFantasma;


// ----------- Funciones --------------
int laberinto();
int leerDatosDeArchivo(int *, int *, int *, int *, int *, int *);
char ** creacionYPosicionamientoLab(Pos *, Pos *, Pos *, int , int );
int bfs(char **, int, int, Pos, Pos);
Pos posRandomVidasyPuntos(int, int);
void mostrarGameOver();

#endif // LABERINTO_H_INCLUDED
