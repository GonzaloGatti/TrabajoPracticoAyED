#include "lista.h"

void crearLista(tLista *lista)
{
    *lista = NULL;
}

int insertarOrdenadoOAcumularLista(tLista *lista, void *info, unsigned tamInfo, int cmp(void *, void *), void acumular(void *, void *))
{
    int resCmp;

    // avanzar mientras el actual sea menor que el nuevo
    while (*lista && (resCmp = cmp((*lista)->info, (void*)info)) < 0)
        lista = &(*lista)->sig;

    // si es igual, acumular in-place y salir
    if (*lista && resCmp == 0) {
        acumular((*lista)->info, (void*)info);
        return EXITO;
    }

    // insertar antes de *lista (al final si *lista==NULL)
    tNodoLista *nue = (tNodoLista*)malloc(sizeof(tNodoLista));
    if (!nue) return ERROR_TOMAR_MEMORIA;

    nue->info = malloc(tamInfo);
    if (!nue->info)
    {
        free(nue);
        return ERROR_TOMAR_MEMORIA;
    }

    memcpy(nue->info, info, tamInfo);
    nue->tamInfo = tamInfo;
    nue->sig = *lista;
    *lista = nue;

    return EXITO;
}

int sacarDeLista(tLista *lista, void *info, unsigned tamInfo)
{
    tNodoLista *aux;

    if(!*lista)
        return LISTA_VACIA;

    aux = *lista;

    memcpy(info, aux->info, MIN(tamInfo, aux->tamInfo));

    *lista = aux->sig;

    free(aux->info);
    free(aux);

    return EXITO;
}

int ordenarLista(tLista *lista, int cmp(void *, void *))
{
    tLista *primero;
    tNodoLista *aux;

    if(!*lista)
        return LISTA_VACIA;

    while(*lista)
    {
        if((primero = devolverPrimero(lista, cmp)) != lista)
        {
            aux = *primero;
            *primero = aux->sig;

            aux->sig = *lista;
            *lista = aux;
        }

        lista = &(*lista)->sig;
    }

    return EXITO;
}

tLista *devolverPrimero(tLista *lista, int cmp(void *, void *))
{
    tLista *primero;

    if(!*lista)
        return NULL;

    primero = lista;
    lista = &(*lista)->sig;

    while(*lista){
        if(cmp((*lista)->info, (*primero)->info) > 0)
            primero = lista;

        lista = &(*lista)->sig;
    }

    return primero;
}

int contarNodosLista(tLista *lista)
{
    int contador = 0;

    while(*lista)
    {
        contador++;
        lista = &(*lista)->sig;
    }

    return contador;
}

void vaciarLista(tLista *lista)
{
    tNodoLista *aux;

    if(!*lista)
        return;

    while(*lista)
    {
        aux = *lista;
        *lista = aux->sig;

        free(aux->info);
        free(aux);
    }
}
