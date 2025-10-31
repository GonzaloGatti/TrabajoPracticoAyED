#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define MIN(a, b) ((a)<(b)?(a):(b))

#define EXITO 0
#define ERROR_TOMAR_MEMORIA 1
#define COLA_VACIA 2
#define ESPACIO_DISPONIBLE 3

// ----------- Estructuras --------------
typedef struct sNodo{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
} tNodo;

typedef struct {
    tNodo *pri, *ult;
} tCola;

// ----------- Primitivas --------------
void crearCola(tCola *);
int ponerEnCola(tCola *, const void *, unsigned);
int verCola(const tCola *, void *, unsigned);
int sacarDeCola(tCola *, void *, unsigned);
void vaciarCola(tCola *);
int colaVacia(tCola *);

#endif // COLA_H_INCLUDED
