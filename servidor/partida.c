#include "partida.h"


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

void acumularPuntos(void *elemento1, void *elemento2)
{
    tRanking *ranking1 = (tRanking *)elemento1, *ranking2 = (tRanking *)elemento2;

    ranking1->totalPuntos += ranking2->totalPuntos;
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

void extraerJugadorAIndice(void *registroCrudo, void *indiceParam){
    tJugador *registro = (tJugador *)registroCrudo;
    tIndice *indice = (tIndice *)indiceParam;

    strcpy(indice->nombreJugador, registro->nombre);
    indice->idJugador = registro->idJugador;
}


// FUNCIONES DE COMPARACION

int cmpIndicePorId(void *e1, void *e2) {
    tIndice *i1 = (tIndice *)e1;
    tIndice *i2 = (tIndice *)e2;

    if (i1->idJugador > i2->idJugador) return 1;
    if (i1->idJugador < i2->idJugador) return -1;
    return 0;
}

int cmpUsuarios(void *elemento1, void *elemento2){
    tIndice *indiceBuscado = (tIndice *)elemento1;
    tIndice *indiceArbol = (tIndice *)elemento2;

    return strcmp(indiceArbol->nombreJugador, indiceBuscado->nombreJugador);
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

