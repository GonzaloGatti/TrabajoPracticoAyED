#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "common.h"
#include "arbol.h"
#include "lista.h"

int guardarPartidaArchivo(char *nombreArchivo, tPartidaSrv *partida);
int modificarCantPartidasJugador(char *nombreArchivo, const tIndice indiceJugador, tPartidaSrv *payloadSrv);
int crearRanking(tLista *lista, char *nombreArchivo);
void acumularPuntos(void *elemento1, void *elemento2);
int obtenerProximoId(const char *nombreArchivo, int *proximoId);
void extraerJugadorAIndice(void *registroCrudo, void *indiceParam);


int cmpIndicePorId(void *e1, void *e2);
int cmpUsuarios(void *elemento1, void *elemento2);
int cmpId(void *elemento1, void *elemento2);
int cmpPuntos(void *elemento1, void *elemento2);

#endif // PARTIDA_H_INCLUDED
