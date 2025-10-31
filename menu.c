#include "menu.h"


void menuJuego(){
    char op, nombreUsuario[LARGO_NOMBRE_USUARIO];
    const char opciones [][LARGO_MENU] = {
                                          "ABC",
                                          "Ver ranking de jugadores",
                                          "Jugar nueva partida",
                                          "Salir del juego"
                                        };


    do{

        op=menu(opciones, "Juego - Laberintos y Fantasmas\n \t\tMenu Principal");

        switch(op){

            case 'A':

                break;

            case 'B':
                system("cls");
                printf("\n Ingrese su nombre de usuario: ");
                scanf("%s", nombreUsuario);
                system("cls");

                laberinto();
                break;
        }

    } while(op != 'C');

 }

char menu(const char menu[][LARGO_MENU], const char* titulo){
    char op;

    op = opcion(menu, titulo, "Ingrese opcion: ");
    while(!strchr(menu[0], op))
        op = opcion(menu, titulo, "Opcion erronea. Ingrese nuevamente: ");

    return op;
}

char opcion(const char menu[][LARGO_MENU], const char* titulo, const char* msj){
    char op;
    int i;

    system("cls");
    printf("\n\t%s \n", titulo);
    for(i=1; i<=strlen(menu[0]); i++)
        printf("\n %c - %s", menu[0][i-1], menu[i]);
    printf("\n\n%s: ", msj);
    fflush(stdin);
    scanf("%c", &op);

    return toupper(op);
}
