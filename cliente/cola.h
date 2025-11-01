#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "common.h"

#include <time.h>
#include <conio.h>
#include <ctype.h>


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
