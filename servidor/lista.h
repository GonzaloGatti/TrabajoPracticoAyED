#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "common.h"

typedef struct sNodoLista{
    void *info;
    unsigned tamInfo;
    struct sNodoLista *sig;
} tNodoLista;

typedef tNodoLista* tLista;

typedef int (*tAccion)(void *, void *);

void crearLista(tLista *);
int insertarOrdenadoOAcumularLista(tLista *, void *, unsigned, int(void *, void *), void(void *, void *));
int sacarDeLista(tLista *, void *, unsigned);
int ordenarLista(tLista *, int(void *, void *));
tLista *devolverPrimero(tLista *, int(void *, void *));
int contarNodosLista(tLista *);
void vaciarLista(tLista *);

#endif // LISTA_H_INCLUDED
