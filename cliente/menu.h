#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "common.h"

#include "laberinto.h"
#include "client_lib.h"

void menuJuego();
char menu(const char[][LARGO_MENU], const char*);
char opcion(const char[][LARGO_MENU], const char*, const char*);


void normalizacion(char *);

#endif // MENU_H_INCLUDED
