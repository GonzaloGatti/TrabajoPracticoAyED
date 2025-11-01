#ifndef SERVIDOR_LIB_H_INCLUDED
#define SERVIDOR_LIB_H_INCLUDED

#include "common.h"

#include "cola.h"

#include <ctype.h>
#include <time.h>
#include <winsock2.h>

// Inicializa Winsock
int init_winsock();

// Crea el socket del servidor
SOCKET create_server_socket();

// Procesa el mensaje recibido y genera la respuesta
void process_request(const char *request, char *response);

// Ejecuta el bucle principal del servidor
void run_server();

#endif // SERVIDOR_LIB_H_INCLUDED
