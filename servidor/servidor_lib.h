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

int existeArchivo(const char *);
int insercionArchivoDesYArbolBalanceado(tArbol *, tPartidaSrv *, char *, char *, int *);

int enviarRanking(SOCKET, tLista *);
int send_all(SOCKET, const void *, int);

#endif // SERVIDOR_LIB_H_INCLUDED
