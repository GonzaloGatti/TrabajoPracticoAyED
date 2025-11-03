#ifndef SERVIDOR_LIB_H_INCLUDED
#define SERVIDOR_LIB_H_INCLUDED

#include "common.h"

#include "cola.h"
#include "arbol.h"
#include "lista.h"

#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include <sys/stat.h>
#include <stdint.h>


// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Ejecuta el bucle principal del servidor
void run_server();

int cmpIndicePorId(void *, void *);
void extraerJugadorAIndice(void *, void *);
int existeArchivo(const char *);
int cmpUsuarios(void *, void *);
int insercionArchivoDesYArbolBalanceado(tArbol *, tPartidaSrv *, char *, char *, int *);
int obtenerProximoId(const char *, int *);
int guardarPartidaArchivo(char *, tPartidaSrv *);

int enviarRanking(SOCKET, tLista *);
int send_all(SOCKET, const void *, int);

int modificarCantPartidasJugador(char *, const tIndice, tPartidaSrv *);

int crearRanking(tLista *, char *);
int cmpId(void *, void *);
int cmpPuntos(void *, void *);
void acumularPuntos(void *, void *);

#endif // SERVIDOR_LIB_H_INCLUDED
