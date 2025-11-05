#include "cola.h"

void crearCola(tCola *cola){
    cola->pri = NULL;
    cola->ult = NULL;
}

int ponerEnCola(tCola *cola, const void *info, unsigned tamInfo){
    tNodo *nue;

    if(!(nue = (tNodo*)malloc(sizeof(tNodo))))
        return ERROR_TOMAR_MEMORIA;

    if(!(nue->info = malloc(tamInfo)))
        return ERROR_TOMAR_MEMORIA;

    //Le asignamos la informacion al nodo nuevo
    memcpy(nue->info, info, tamInfo);
    nue->tamInfo = tamInfo;
    nue->sig = NULL;

    //Si hay ultimo, conectamos el nuevo al ultimo
    if(cola->ult)
        cola->ult->sig = nue;
    //Si no hay ultimo (no hay nada en la cola), el primero es el nuevo
    else
        cola->pri = nue;

    //Actualizamos al ultimo, que pasa a ser el nuevo. Si no habia ultimo
    //primero y ultimo son el mismo nodo (unico elemento)
    cola->ult = nue;

    return EXITO;
}

int verCola(const tCola *cola, void *info, unsigned tamInfo){

    if(!cola->pri)
        return COLA_VACIA;

    memcpy(info, cola->pri->info, MIN(tamInfo, cola->pri->tamInfo));

    return EXITO;
}

int sacarDeCola(tCola *cola, void *info, unsigned tamInfo){
    tNodo *aux = cola->pri;

    if(!aux)
        return COLA_VACIA;

    memcpy(info, aux->info, MIN(aux->tamInfo, tamInfo));

    cola->pri = aux->sig;
    if(!cola->pri)
        cola->ult = NULL;

    free(aux->info);
    free(aux);

    return EXITO;
}

void vaciarCola(tCola *cola){
    tNodo *aux;

    while(cola->pri){
        aux = cola->pri;
        cola->pri = aux->sig;
        free(aux->info);
        free(aux);
    }

    cola->ult = NULL;
}

int colaVacia(tCola *cola){
    if(!cola->pri)
        return COLA_VACIA;

    return ESPACIO_DISPONIBLE;
}
