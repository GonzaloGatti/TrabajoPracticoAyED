#include "client_lib.h"
#include <stdio.h>
#include <string.h>

int init_winsock() {
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET connect_to_server(const char *server_ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        closesocket(sock);
        return INVALID_SOCKET;
    }

    return sock;
}

int send_request(SOCKET sock, tPartidaCli *datos, char *response) {
    if (send(sock, (const char *)datos, sizeof(tPartidaCli), 0) < 0) {
        return -1;
    }

    int bytes_received = recv(sock, response, BUFFER_SIZE - 1, 0);

    if (bytes_received == 0)
        return 1;

    if (bytes_received < 0)
        return -1;

    response[bytes_received] = '\0';
    return EXITO;
}

void close_connection(SOCKET sock) {
    closesocket(sock);
    WSACleanup();
}

int recibirYMostrarRanking(SOCKET srv)
{
    uint32_t n_net;
    int colorPrimero = 1, colorSegundo = 2, colorTercero = 3;
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

        if(colorPrimero)
        {
            printf("\x1b[38;5;220m%2d) %-20s  ID:%4d  Puntos:%6d\x1b[0m\n",
                   i+1, r.nombre, r.idJugador, r.totalPuntos);
            colorPrimero--;
            colorSegundo--;
            colorTercero--;
        } else if(colorSegundo)
        {
            printf("\x1b[38;5;136m%2d) %-20s  ID:%4d  Puntos:%6d\x1b[0m\n",
                   i+1, r.nombre, r.idJugador, r.totalPuntos);

            colorSegundo--;
            colorTercero--;
        } else if(colorTercero)
        {
            printf("\x1b[38;5;7m%2d) %-20s  ID:%4d  Puntos:%6d\x1b[0m\n",
                   i+1, r.nombre, r.idJugador, r.totalPuntos);

            colorTercero--;
        } else
        {
            printf("\x1b[90m%2d) %-20s  ID:%4d  Puntos:%6d\x1b[0m\n",
                   i+1, r.nombre, r.idJugador, r.totalPuntos);
        }
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
