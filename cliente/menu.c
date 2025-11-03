#include "menu.h"

void menuJuego()
{
    tPartidaCli datos;
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
    if (sock == INVALID_SOCKET)
    {
        printf("\n\t\t\tNo se pudo conectar al servidor\n\n");
        printf("\tSe jugara OFFLINE, por lo que sus partidas no seran guardadas\n\n");
        system("pause");
        WSACleanup();
    }

    printf("Conectado al servidor.\n");
    do
    {
        op=menu(opciones, "Juego - Laberintos y Fantasmas\n \t\tMenu Principal");
        datos.opcion = op;

        switch(op){

            case 'A':

                if (send(sock, (const char*)&datos, sizeof(datos), 0) != sizeof(datos))
                {
                    printf("\n\tError enviando solicitud de ranking (servidor desconectado)\n\n");
                    system("pause");
                    break;
                }

                if(!recibirYMostrarRanking(sock))
                   printf("Error recibiendo ranking\n");

                system("pause");

                break;

            case 'B':
                system("cls");
                printf("\n Ingrese su nombre de usuario: ");
                scanf(" %19[^\n]", datos.nombre);
                normalizacion(datos.nombre);
                system("cls");

                if((resultado = laberinto(&datos)) != EXITO)
                    break;

                system("cls");
                if (send_request(sock, &datos, response) == 0 && !resultado)
                    printf("Respuesta: %s\n", response);
                else
                {
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
    printf("\n\n%s ", msj);
    fflush(stdin);
    scanf("%c", &op);

    return toupper(op);
}

int recibirYMostrarRanking(SOCKET srv)
{
    uint32_t n_net;
    if (!recv_all(srv, &n_net, sizeof n_net)) return 0;
    int n = (int)ntohl(n_net);

    if (n <= 0) {
        printf("\n=== RANKING (vacío) ===\n");
        return 1;
    }

    int bytes = n * TamRanking;
    char *buf = (char*)malloc(bytes);
    if (!buf) return 0;

    if (!recv_all(srv, buf, bytes)) { free(buf); return 0; }

    printf("\n\t=== RANKING (TOP %d) ===\n\n", n);
    for (int i = 0; i < n; ++i) {
        int off = i * TamRanking;

        uint32_t id_n, pts_n;
        memcpy(&id_n,  buf + off, 4); off += 4;
        memcpy(&pts_n, buf + off, 4); off += 4;

        tRanking r;
        r.idJugador   = (int)ntohl(id_n);
        r.totalPuntos = (int)ntohl(pts_n);

        memcpy(r.nombre, buf + off, LARGO_NOMBRE_USUARIO);
        r.nombre[LARGO_NOMBRE_USUARIO - 1] = '\0';

        printf("%2d) %-20s  ID:%4d  Puntos:%6d\n",
               i+1, r.nombre, r.idJugador, r.totalPuntos);
    }

    free(buf);
    return 1;
}

int recv_all(SOCKET s, void *buf, int len)
{
    char *p = (char*)buf;
    int recvd = 0;
    while (recvd < len) {
        int n = recv(s, p + recvd, len - recvd, 0);
        if (n <= 0) return 0;   // error o desconexión
        recvd += n;
    }
    return 1;
}

void normalizacion(char *cad) {
    char *src = cad;
    char temp[60];  // buffer temporal seguro
    char *dst = temp;
    int nuevaPalabra = 1;
    int espacioPrevio = 0;

    while (*src && (dst - temp) < (int)sizeof(temp) - 1) {
        // Si el usuario puso una coma lo vamos a ignorar
        if (*src == ',') {
            espacioPrevio = 1;
            nuevaPalabra = 1;
            src++;
            continue;
        }

        //si hay un espacio, empieza otra palabra
        if (isspace((unsigned char)*src)) {
            espacioPrevio = 1;
            nuevaPalabra = 1;
            src++;
            continue;
        }

        //ahora estamos en una palabra
        if (dst != temp && espacioPrevio && (dst - temp) < (int)sizeof(temp) - 1)
                *dst++ = ' ';


        //Cuando es nueva palabra le pongo mayus a la primer letra y despues ya seran minusc, asi q cambio la bandera
        if (nuevaPalabra) {
            *dst++ = toupper((unsigned char)*src);
            nuevaPalabra = 0;
        } else {
            *dst++ = tolower((unsigned char)*src);
        }
        espacioPrevio = 0;
        src++;
    }

    *dst = '\0';

    //copio todo lo que fui haciendo en la cadena temporal a mi cadena original
    strcpy(cad, temp);
    cad[sizeof(temp) - 1] = '\0';
}
