#include "menu.h"
#include "client_lib.h"

void menuJuego(){
    tPartida datos;
    int resultado;
    char response[BUFFER_SIZE];
    char op;
    const char opciones [][LARGO_MENU] = {
                                          "ABC",
                                          "Ver ranking de jugadores",
                                          "Jugar nueva partida",
                                          "Salir del juego"
                                        };

    if (init_winsock() != 0)
        printf("Error al inicializar Winsock\n");

    SOCKET sock = connect_to_server(SERVER_IP, PORT);
    if (sock == INVALID_SOCKET) {
        printf("No se pudo conectar al servidor\n");
        WSACleanup();
    }

    printf("Conectado al servidor.\n");
    do{

        op=menu(opciones, "Juego - Laberintos y Fantasmas\n \t\tMenu Principal");

        switch(op){

            case 'A':

                break;

            case 'B':
                system("cls");
                printf("\n Ingrese su nombre de usuario: ");
                scanf("%s", datos.nombre);
                system("cls");

                resultado = laberinto(&datos);

                system("cls");
                if (send_request(sock, &datos, response) == 0 && !resultado) {
                    printf("Respuesta: %s\n", response);
                } else {
                    printf("Error al enviar o recibir datos\n");
                    break;
                }

                system("pause");
                break;

            case 'C':
                break;
        }

    } while(op != 'C');

     close_connection(sock);
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
