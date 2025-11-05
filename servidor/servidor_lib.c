#include "servidor_lib.h"

void run_server()
{
    char response[128];
    int bytes_received;

    struct sockaddr_in client_addr;
    int client_addr_size;
    int err;

    int hayCambios = 0;
    int proximoIdMem;

    tPartidaCli payload;
    tPartidaSrv payloadSrv;
    tCola cola;
    tIndice indiceBuscar;
    tLista lista;

    tArbol arbol;


    if (init_winsock() != 0)
    {
        printf("Error al inicializar Winsock\n");
        return;
    }

    crearArbol(&arbol);

    // Decidir de dónde levantar el árbol binario de busqueda balanceado
    if (existeArchivo(ARCH_INDICE))
    {
        // Ya tengo índice ordenado, lo levanto balanceado
        if (cargarArbolBalanceadoDeArchIndice(&arbol, ARCH_INDICE, sizeof(tIndice)) != EXITO)
        {
            printf("No se pudo cargar el arbol desde el indice. Regenerando...");

            // Si no se pudo cargar, lo genero de cero
            cargarArbolDeBinDesord(&arbol, ARCH_DATOS, sizeof(tJugador), cmpIndicePorId, extraerJugadorAIndice);
            crearArchBinIndiceDeArbol(&arbol, ARCH_INDICE);
        }

    } else
    {
        // No hay índice, lo genero desde el archivo desordenado
        if (cargarArbolDeBinDesord(&arbol, ARCH_DATOS, sizeof(tJugador), cmpIndicePorId, extraerJugadorAIndice) != EXITO)
            printf("Archivo 'jugadores.dat' vacio o inexistente, se inicializa estructura vacia\n");


        // y lo bajo ordenado para la próxima ejecución
        crearArchBinIndiceDeArbol(&arbol, ARCH_INDICE);

        // Cargamos el arbol binario de busqueda balanceado
        vaciarArbol(&arbol);
        cargarArbolBalanceadoDeArchIndice(&arbol, ARCH_INDICE, sizeof(tIndice));
    }

    // Obtenemos el proximo ID en caso de que se quiera dar el alta de un jugador (ID incremental)
    obtenerProximoId(ARCH_INDICE, &proximoIdMem);

    // Creamos socket servidor
    SOCKET server_socket = create_server_socket();
    if (server_socket == INVALID_SOCKET)
    {
        printf("Error al crear socket del servidor\n");
        WSACleanup();
        return;
    }

    // Lo ponemos a escuchar y aceptamos cliente en caso de haber
    printf("Servidor escuchando en puerto %d...\n", PORT);

    client_addr_size = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);

    if (client_socket == INVALID_SOCKET)
    {
        printf("Error en accept()\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Cliente conectado.\n");

    // Creamos cola para las peticiones
    crearCola(&cola);

    // Bucle principal
    while (1)
    {

        // Intentar recibir un mensaje
        bytes_received = recv(client_socket, (char *)&payload, sizeof(payload), 0);

        // Si hay mensaje, nos fijamos que opcion eligio el jugador (A, B, C) desde el cliente
        if (bytes_received > 0)
        {
            // En caso de ser A, sebemos armar el archivo ranking
            if(toupper(payload.opcion) == 'A')
            {
                crearLista(&lista);

                crearRanking(&lista, ARCH_PARTIDAS);
                ordenarLista(&lista, cmpPuntos);

                if(enviarRanking(client_socket, &lista) != EXITO)
                {
                    printf("Error aca");
                }

                vaciarLista(&lista);
            }
                // En caso de ser B, analizamos si el jugador que envio la partida existe o no y damos su alta. Ademas, cargamos la partida en el archivo de partidas
                else if(toupper(payload.opcion) == 'B')
            {
                printf("[RX] Recibido: Nombre: %s, Cantidad de puntos: %d, Cantidad de movimientos: %d\n", payload.nombre, payload.cantidadPuntos, payload.cantidadMovimientos);

                // LLevamos los datos de la partida a la estructura del servidor (distinta a la del cliente)
                strcpy(payloadSrv.nombre, payload.nombre);
                payloadSrv.cantidadPuntos = payload.cantidadPuntos;
                payloadSrv.cantidadMovimientos = payload.cantidadMovimientos;

                // Encolamos el mensaje recibido
                ponerEnCola(&cola, &payloadSrv, sizeof(payloadSrv));
            }

        } else if (bytes_received == 0)
        {
            // El cliente ha cerrado la conexión de forma ordenada
            printf("Cliente desconectado (recv == 0). Saliendo del bucle.\n");
            break;

        } else /* bytes_received == SOCKET_ERROR */
        {
            err = WSAGetLastError();        // Ver que es esto
            if (err == WSAEWOULDBLOCK)
            {
                // No hay datos en modo no-bloqueante.
                // Simplemente seguimos el bucle y procesamos la cola.
            } else
            {
                // Error serio, salimos
                printf("Error en recv(): %d. Saliendo.\n", err);
                break;
            }
        }

        // Procesar mensajes pendientes en la cola
        while (colaVacia(&cola) != COLA_VACIA)
        {
            // Desencolar la peticion
            sacarDeCola(&cola, &payloadSrv, sizeof(payloadSrv));

            // Copiamos el nombre a una nueva variable tipo indice, asi hacemos la busqueda en el arbol balanceado, y en caso de tener un hit,
            // copiamos directamente todo el nodo encontrado a este indice
            memset(&indiceBuscar, 0, sizeof(indiceBuscar));
            strcpy(indiceBuscar.nombreJugador, payloadSrv.nombre);

            // Hacemos dicha busqueda en el arbol
            if(elementoEstaEnArbol(&arbol, &indiceBuscar, sizeof(indiceBuscar), cmpUsuarios) == ELEMENTO_NO_ENCONTRADO)
            {
                // En caso de que el jugador no se encuentre dado de alta, intentamos darlo de alta tanto en el archivo de jugadores desordenados,
                // como en el arbol balanceado (en el archivo indice lo persistimos luego) (usamos el proximo ID tomado anterioremente)
                if(insercionArchivoDesYArbolBalanceado(&arbol, &payloadSrv, ARCH_DATOS, ARCH_INDICE, &proximoIdMem) != EXITO)
                    strcpy(response, "Error al dar el alta");
                else
                {
                    // En el caso de darlo de alta exitosamente, enviamos mensaje y cambiamos el flag "hayCambios" a 1, que sirve para saber al final
                    // de la ejecucion del servidor si hay que regenerar el archivo indice
                    strcpy(response, "Jugador dado de alta correctamente");
                    hayCambios = 1;
                }

            } else
            {
                modificarCantPartidasJugador(ARCH_DATOS, indiceBuscar, &payloadSrv);

                // Si el jugador ya existia, tomamos su ID del arbol balanceado
                payloadSrv.idJugador = indiceBuscar.idJugador;
                strcpy(response, "El jugador ya se encontraba en la base de datos");
            }

            guardarPartidaArchivo(ARCH_PARTIDAS, &payloadSrv);

            // Enviar la respuesta al cliente (si es necesario)
            send(client_socket, response, strlen(response), 0);
            printf("[TX] Enviado: %s\n", response);
        }


        // Evita usar de manera intenciva la CPU
        Sleep(100);
    }

    if(hayCambios)
        crearArchBinIndiceDeArbol(&arbol, ARCH_INDICE);


    vaciarCola(&cola);
    printf("Conexion cerrada.\n");
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
}

// Implementación de funciones públicas
int init_winsock()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

SOCKET create_server_socket()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return INVALID_SOCKET;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    if (listen(s, 1) == SOCKET_ERROR)
    {
        closesocket(s);
        return INVALID_SOCKET;
    }

    return s;
}

int cmpIndicePorId(void *e1, void *e2) {
    tIndice *i1 = (tIndice *)e1;
    tIndice *i2 = (tIndice *)e2;

    if (i1->idJugador > i2->idJugador) return 1;
    if (i1->idJugador < i2->idJugador) return -1;
    return 0;
}

void extraerJugadorAIndice(void *registroCrudo, void *indiceParam){
    tJugador *registro = (tJugador *)registroCrudo;
    tIndice *indice = (tIndice *)indiceParam;

    strcpy(indice->nombreJugador, registro->nombre);
    indice->idJugador = registro->idJugador;
}

int existeArchivo(const char *nombre)
{
    struct stat st;
    return stat(nombre, &st) == 0;
}

int cmpUsuarios(void *elemento1, void *elemento2){
    tIndice *indiceBuscado = (tIndice *)elemento1;
    tIndice *indiceArbol = (tIndice *)elemento2;

    return strcmp(indiceArbol->nombreJugador, indiceBuscado->nombreJugador);
}

int insercionArchivoDesYArbolBalanceado(tArbol *arbol, tPartidaSrv *partida, char *nombreArchDesordenado, char *nombreArchivoIndice, int *proxIdMem){
    FILE *archivo;
    tIndice nuevoRegIndice;
    int posicion, idNuevo;
    long offset;
    tJugador jugador;

    idNuevo = *proxIdMem;
    partida->idJugador = idNuevo;
    partida->cantidadPartidas = 1;

    jugador.idJugador = idNuevo;
    jugador.cantidadPartidas = 1;
    strcpy(jugador.nombre, partida->nombre);

    if(!(archivo = fopen(nombreArchDesordenado, "ab")))
        return ERROR_ARCHIVO;

    if(fwrite(&jugador, sizeof(jugador), 1, archivo) != 1){
        fclose(archivo);
        return ERROR_ARCHIVO;
    }

    offset = ftell(archivo);
    posicion = (int)(offset / sizeof(tJugador)) - 1;

    fclose(archivo);


    nuevoRegIndice.idJugador = idNuevo;
    nuevoRegIndice.posicion  = posicion;
    strcpy(nuevoRegIndice.nombreJugador, jugador.nombre);

    if (insertarArbolR(arbol, &nuevoRegIndice, sizeof(nuevoRegIndice), cmpIndicePorId) != EXITO)
        return ERROR_INSERTAR_EN_ARBOL;

    (*proxIdMem)++;

    return EXITO;
}

int obtenerProximoId(const char *nombreArchivo, int *proximoId)
{
    FILE *archivo;
    tIndice ultimo;

    archivo = fopen(nombreArchivo, "rb");
    if (!archivo) {
        // No existe todavía el índice, primer id
        *proximoId = 1;
        return EXITO;
    }

    // Ir al último registro
    if (fseek(archivo, -(long)sizeof(tIndice), SEEK_END) != 0) {
        fclose(archivo);
        *proximoId = 1;
        return EXITO;
    }

    if (fread(&ultimo, sizeof(tIndice), 1, archivo) != 1) {
        fclose(archivo);
        *proximoId = 1;
        return EXITO;
    }

    fclose(archivo);
    *proximoId = ultimo.idJugador + 1;

    return EXITO;
}

int guardarPartidaArchivo(char *nombreArchivo, tPartidaSrv *partida)
{
    FILE *archivoPartidas;

    if(!(archivoPartidas = fopen(nombreArchivo, "ab")))
    {
        printf("Se produjo un error al abrir el archivo de partidas");
        return ERROR_ARCHIVO;
    }

    if(fwrite(partida, sizeof(*partida), 1, archivoPartidas) != 1)
    {
        printf("Error al guardar partida");
        fclose(archivoPartidas);
        return ERROR_ARCHIVO;
    }

    if (fflush(archivoPartidas) != 0) {
        printf("Error al guardar partida");
        fclose(archivoPartidas);
        return ERROR_ARCHIVO;
    }

    fclose(archivoPartidas);

    return EXITO;
}

int enviarRanking(SOCKET cli, tLista *lista)
{
    // contar todos
    int n = contarNodosLista(lista);

    // 1) enviar cantidad
    uint32_t n_net = htonl((uint32_t)n);
    if (send_all(cli, &n_net, sizeof n_net) != EXITO)
        return ERROR_SOCKET;

    if (n == 0)
        return EXITO;

    // 2) armar buffer plano y enviar de una
    int bytes = n * TAM_RANKING;
    char *buf = (char*)malloc(bytes);
    if (!buf) return ERROR_TOMAR_MEMORIA;

    tLista it = *lista;
    for (int i = 0; i < n && it; ++i, it = it->sig) {
        const tRanking *r = (const tRanking*)it->info;

        uint32_t id_n  = htonl((uint32_t)r->idJugador);
        uint32_t pts_n = htonl((uint32_t)r->totalPuntos);

        int off = i * TAM_RANKING;
        memcpy(buf + off, &id_n,  4); off += 4;
        memcpy(buf + off, &pts_n, 4); off += 4;

        // nombre fijo TAM_NOMBRE bytes
        memset(buf + off, 0, TAM_NOMBRE);
        strncpy(buf + off, r->nombre, TAM_NOMBRE - 1);
    }

    int st = send_all(cli, buf, bytes);
    free(buf);
    return (st == EXITO) ? EXITO : ERROR_SOCKET;
}

int send_all(SOCKET s, const void *buf, int len) {
    const char *p = (const char*)buf;
    int sent = 0;
    while (sent < len) {
        int n = send(s, p + sent, len - sent, 0);
        if (n <= 0) return ERROR_SOCKET;
        sent += n;
    }
    return EXITO;
}

int modificarCantPartidasJugador(char *nombreArchivo, const tIndice indiceJugador, tPartidaSrv *payloadSrv){
    FILE *archivo;
    tJugador jugador;
    long pos = (long)indiceJugador.posicion;
    long offset = pos * (long)sizeof(tJugador);

    if(!(archivo = fopen(nombreArchivo, "r+b")))
        return ERROR_ARCHIVO;

    if (fseek(archivo, offset, SEEK_SET) != 0)
    {
        fclose(archivo);
        return ERROR_ARCHIVO;
    }

    if (fread(&jugador, sizeof(jugador), 1, archivo) != 1)
    {
        fclose(archivo);
        return ERROR_ARCHIVO;
    }

    jugador.cantidadPartidas++;
    payloadSrv->cantidadPartidas = jugador.cantidadPartidas;

    if (fseek(archivo, offset, SEEK_SET) != 0)
    {
        fclose(archivo);
        return ERROR_ARCHIVO;
    }

    if (fwrite(&jugador, sizeof(jugador), 1, archivo) != 1)
    {
        fclose(archivo);
        return ERROR_ARCHIVO;
    }

    fclose(archivo);

    return EXITO;
}

int crearRanking(tLista *lista, char *nombreArchivo)
{
    FILE *archivo;
    tRanking ranking;
    tPartidaSrv partida;
    int res;

    if(!(archivo = fopen(nombreArchivo, "rb")))
        return ERROR_ARCHIVO;

    while(fread(&partida, sizeof(partida), 1, archivo) == 1)
    {
        ranking.idJugador = partida.idJugador;
        strcpy(ranking.nombre, partida.nombre);
        ranking.totalPuntos = partida.cantidadPuntos;

        if((res =insertarOrdenadoOAcumularLista(lista, &ranking, sizeof(ranking), cmpId, acumularPuntos)) != EXITO)
        {
            fclose(archivo);
            return res;
        }
    }

    fclose(archivo);

    return EXITO;
}

int cmpId(void *elemento1, void *elemento2)
{
    tRanking *ranking1 = (tRanking *)elemento1, *ranking2 = (tRanking *)elemento2;

    return ranking1->idJugador - ranking2->idJugador;
}

int cmpPuntos(void *elemento1, void *elemento2)
{
    tRanking *ranking1 = (tRanking *)elemento1, *ranking2 = (tRanking *)elemento2;

    return ranking1->totalPuntos - ranking2->totalPuntos;
}

void acumularPuntos(void *elemento1, void *elemento2)
{
    tRanking *ranking1 = (tRanking *)elemento1, *ranking2 = (tRanking *)elemento2;

    ranking1->totalPuntos += ranking2->totalPuntos;
}
