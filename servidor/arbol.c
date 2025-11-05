#include "arbol.h"


void crearArbol(tArbol *arbol){
    *arbol = NULL;
}

int insertarArbolR(tArbol *arbol, void *info, unsigned tamInfo, int cmp(void*, void*)){
    tNodoArbol *nodoAux;
    int resCmp;

    if(*arbol){
        if((resCmp = cmp(info, (*arbol)->info)) > 0)
           return insertarArbolR(&((*arbol)->der), info, tamInfo, cmp);
        else if(resCmp < 0)
            return insertarArbolR(&((*arbol)->izq), info, tamInfo, cmp);
        else
            return ELEMENTO_REPETIDO;
    }

    if(!(nodoAux = (tNodoArbol *)malloc(sizeof(tNodoArbol))))
        return ERROR_TOMAR_MEMORIA;

    if(!(nodoAux->info = malloc(tamInfo))){
        free(nodoAux);
        return ERROR_TOMAR_MEMORIA;
    }

    memcpy(nodoAux->info, info, tamInfo);
    nodoAux->tamInfo = tamInfo;
    nodoAux->izq = NULL;
    nodoAux->der = NULL;

    *arbol = nodoAux;

    return EXITO;
}

int cargarArbolDeBinDesord(tArbol *arbol, char *nombreArchivo, unsigned tamInfo, int cmpClave(void *, void *), tExtraer extraerDatos){
    tIndice indice;
    FILE *archivo;
    void *buffer;

    if(!(archivo = fopen(nombreArchivo, "rb")))
        return ARCHIVO_NO_EXISTE;

    if(!(buffer = malloc(tamInfo))){
        fclose(archivo);
        return ERROR_TOMAR_MEMORIA;
    }

    indice.posicion = 0;

    while(fread(buffer, tamInfo, 1, archivo) == 1){
        extraerDatos(buffer, &indice);

        if(insertarArbolR(arbol, &indice, sizeof(indice), cmpClave) != EXITO){
            free(buffer);
            fclose(archivo);
            return ERROR_INSERCION;
        }

        indice.posicion++;
    }

    fclose(archivo);
    free(buffer);

    return EXITO;
}

int crearArchBinIndiceDeArbol(tArbol *arbol, char *nombreArchivo){
    FILE *archivo;

    if(!(archivo = fopen(nombreArchivo, "wb")))
        return ERROR_ARCHIVO;

    recorrerArbolInOrdenR(arbol, _insertarRegistroEnArchivoIndice, archivo);


    fclose(archivo);

    return EXITO;
}

void _insertarRegistroEnArchivoIndice(void *info, unsigned tamInfo, void *param){
    fwrite(info, tamInfo, 1, param);
}

int cargarArbolBalanceadoDeArchIndice(tArbol *arbol, char *nombreArchivo, unsigned tamInfo){
    FILE *archivo;
    int res;

    if(!(archivo = fopen(nombreArchivo, "rb")))
        return ERROR_ARCHIVO;

    fseek(archivo, 0L, SEEK_END);
    res = _cargarArbolBalanceadoDeArchIndice(arbol, archivo, tamInfo, 0, ftell(archivo)/tamInfo-1);

    fclose(archivo);

    return res;
}

int _cargarArbolBalanceadoDeArchIndice(tArbol *arbol, FILE *archivo, unsigned tamInfo, int li, int ls){
    int subArbolIzq, subArbolDer;
    int medio;
    void *buffer;

    // Caso base
    if(li > ls)
        return EXITO;

    // Calculamos la posicion del medio
    medio = (ls + li)/2;

    // Tomamos memoria para leer el registro del archivo
    if(!(buffer = malloc(tamInfo)))
        return ERROR_TOMAR_MEMORIA;

    // Leemos el elemento del medio en el archivo
    fseek(archivo, medio * tamInfo, SEEK_SET);
    if(fread(buffer, tamInfo, 1, archivo) != 1){
        free(buffer);
        return ERROR_ARCHIVO;
    }

    // Reservamos espacio para el nodo e insertamos el elemento del medio
    if(!((*arbol) = malloc(sizeof(tNodoArbol)))){
        free(buffer);
        return ERROR_TOMAR_MEMORIA;
    }

    (*arbol)->info = buffer;
    (*arbol)->tamInfo = tamInfo;
    (*arbol)->izq = NULL;
    (*arbol)->der = NULL;

    // Aplicamos lo mismo para los subarboles derecho e izquierdo
    subArbolIzq = _cargarArbolBalanceadoDeArchIndice(&(*arbol)->izq, archivo, tamInfo, li, medio - 1);
    subArbolDer = _cargarArbolBalanceadoDeArchIndice(&(*arbol)->der, archivo, tamInfo, medio + 1, ls);

    // Devuelvo el error del subarbol izquierdo si lo hubo, sino el del derecho.
    // Si no hubo error en ninguno, se devuelve EXITO
    return subArbolIzq != EXITO ? subArbolIzq : subArbolDer;
}

void vaciarArbol(tArbol* arbol)
{
    // Si el árbol está vacío, no hay nada que hacer
    if (!*arbol)
    return;

    /* I */ vaciarArbol( &(*arbol)->izq);
    /* D */ vaciarArbol( &(*arbol)->der);
    /* R */ free((*arbol)->info);

    free(*arbol);

    *arbol = NULL;
}

int elementoEstaEnArbol(tArbol *arbol, void *info, unsigned tamInfo, int cmp(void *, void *)){
    int res;

    if (!*arbol)
        return ELEMENTO_NO_ENCONTRADO;    // no encontrado

    // 1) buscar en el subárbol izquierdo
    res = elementoEstaEnArbol(&(*arbol)->izq, info, tamInfo, cmp);
    if (res == ELEMENTO_ENCONTRADO)
        return ELEMENTO_ENCONTRADO;    // ya lo encontré abajo, no sigo

    // 2) comparar con ESTE nodo
    if (cmp(info, (*arbol)->info) == 0) {
        // lo encontré → copio el nodo al buffer del caller
        memcpy(info, (*arbol)->info, MIN(tamInfo, (*arbol)->tamInfo));
        return ELEMENTO_ENCONTRADO;
    }

    // 3) buscar en el subárbol derecho
    return elementoEstaEnArbol(&(*arbol)->der, info, tamInfo, cmp);
}
