#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "common.h"


typedef struct sNodoArbol{
    void *info;
    unsigned tamInfo;
    struct sNodoArbol *izq;
    struct sNodoArbol *der;
} tNodoArbol;

typedef tNodoArbol* tArbol;

typedef struct{
    int idJugador;
    unsigned posicion;
    char nombreJugador[20];
} tIndice;

typedef void(*tExtraer)(void *, void *);

void crearArbol(tArbol *);
int insertarArbolR(tArbol *, void *, unsigned, int(void*, void*));
int insertarHojaI(tArbol *, void *, unsigned, int(void*, void*));
tArbol *buscarElemento(tArbol *, void *, unsigned, int(void *, void *));

void recorrerArbolPreOrdenR(tArbol *, void(void *));
void recorrerArbolInOrdenR(tArbol *, void(void *, unsigned, void *), void *);
void recorrerArbolPostOrdenR(tArbol *, void(void *));

int contarHojas(tArbol *);
int cantidadNodos(tArbol *);

tArbol *buscarNodoClaveMayor(tArbol *);
tArbol *buscarNodoClaveMenor(tArbol *);

int cantNodosSubDerCondicion(tArbol *, void *, int(void *, void *));
int contarNodosCondicion(tArbol *, void *, int(void *, void *));
float promedioNodosClavePar(tArbol *, int(void *), void(int *, void *));
int _promedioNodosClavePar(tArbol *, int *, int(void *), void(int *, void *));

int saberAltura(tArbol *);

void verHastaAltura(tArbol *, int, void(void *));
void verNodosUnaAltura(tArbol *, int, void(void *));
void verNodosDesdeAltura(tArbol *, int, void(void *));

int eliminarNodo(tArbol *, void *, unsigned, int(void *, void *));
int eliminarRaiz(tArbol *);

int esArbolCompletoEnAltura(tArbol *, int);
int esArbolCompleto(tArbol *);

int cargarArbolDeBinDesord(tArbol *, char *, unsigned, int(void *, void *), tExtraer);
int crearArchIndiceDeArbol(tArbol *, char *, tExtraer);
void _insertarRegistroEnArchivoIndice(void *, unsigned, void *);
int cargarArbolBalanceadoDeArchIndice(tArbol *, char *, unsigned);
int _cargarArbolBalanceadoDeArchIndice(tArbol *, FILE *, unsigned, int, int);
int crearArchBinIndiceDeArbol(tArbol *, char *);

void vaciarArbol(tArbol *);

int elementoEstaEnArbol(tArbol *arbol, void *, unsigned, int(void *, void *));

#endif // ARBOL_H_INCLUDED
