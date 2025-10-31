#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "laberinto.h"

#define LARGO_MENU 30
#define LARGO_NOMBRE_USUARIO 30

void menuJuego();
char menu(const char[][LARGO_MENU], const char*);
char opcion(const char[][LARGO_MENU], const char*, const char*);

#endif // MENU_H_INCLUDED
