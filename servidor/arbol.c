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

int insertarHojaI(tArbol *arbol, void *info, unsigned tamInfo, int cmp(void*, void*)){
    tNodoArbol *nodoAux;
    int resCmp;

    while(*arbol){
        if((resCmp = cmp(info, (*arbol)->info)) > 0)
           arbol = &((*arbol)->der);
        else if(resCmp < 0)
            arbol = &((*arbol)->izq);
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

tArbol *buscarElemento(tArbol *arbol, void *campo, unsigned tamInfo, int cmp(void *, void *)){
    int resCmp;

    if(!*arbol)
        return NULL;

    resCmp = cmp(campo, (*arbol)->info);


    if(resCmp > 0)
       return buscarElemento(&((*arbol)->der), campo, tamInfo, cmp);
    else if(resCmp < 0)
        return buscarElemento(&((*arbol)->izq), campo, tamInfo, cmp);


    memcpy(campo, (*arbol)->info, (*arbol)->tamInfo);
    tamInfo = (*arbol)->tamInfo;

    return arbol;
}

void recorrerArbolPreOrdenR(tArbol *arbol, void print(void *)){

    if(!*arbol) // --> Caso base
        return;

    print((*arbol)->info);

    recorrerArbolPreOrdenR(&((*arbol)->izq), print);
    recorrerArbolPreOrdenR(&((*arbol)->der), print);
}

void recorrerArbolInOrdenR(tArbol *arbol, void accion(void *, unsigned, void *), void *param){

    if(!*arbol) // --> Caso base
        return;

    recorrerArbolInOrdenR(&((*arbol)->izq), accion, param);
    accion((*arbol)->info, (*arbol)->tamInfo, param);
    recorrerArbolInOrdenR(&((*arbol)->der), accion, param);
}

void recorrerArbolPostOrdenR(tArbol *arbol, void print(void *)){

    if(!*arbol) // --> Caso base
        return;

    recorrerArbolPostOrdenR(&((*arbol)->izq), print);
    recorrerArbolPostOrdenR(&((*arbol)->der), print);
    print((*arbol)->info);
}

int contarHojas(tArbol *arbol){

    // Caso base, arbol vacio (no hay hojas)
    if(!*arbol)
        return 0;

    // Estamos en una hoja
    if((*arbol)->izq == NULL && (*arbol)->der == NULL)
        return 1;

    // Si estamos en una no hoja, sumamos las hojas de los subarboles
    // derecho e izquierdo
    return contarHojas(&((*arbol)->izq)) + contarHojas(&((*arbol)->der));
}


int cantidadNodos(tArbol *arbol){

    // Caso base, arbol vacio (no hay nodo)
    if(!*arbol)
        return 0;

    // En caso de que estemos parados sobre un nodo, lo contamos y sumamos
    // los posibles nodos de los subarboles derechos e izquierdos
    return cantidadNodos(&((*arbol)->izq)) + cantidadNodos(&((*arbol)->der)) + 1;
}

tArbol *buscarNodoClaveMayor(tArbol *arbol){

    if(!*arbol)
        return NULL;

    while((*arbol)->der)
        arbol = &((*arbol)->der);

    return arbol;
}

tArbol *buscarNodoClaveMenor(tArbol *arbol){

    if(!*arbol)
        return NULL;

    while((*arbol)->izq)
        arbol = &((*arbol)->izq);

    return arbol;
}

int cantNodosSubDerCondicion(tArbol *arbol, void *condicion, int cmp(void *, void *)){

    if(!*arbol)
        return 0;

    arbol = &((*arbol)->der);

    return contarNodosCondicion(arbol, condicion, cmp);
}

int contarNodosCondicion(tArbol *arbol, void *condicion, int cmp(void *, void *)){

    // Caso base
    if(!*arbol)
        return 0;

    if(cmp(condicion, (*arbol)->info) == 0)
        return contarNodosCondicion(&((*arbol)->der), condicion, cmp) + contarNodosCondicion(&((*arbol)->izq), condicion, cmp) + 1;
    else
        return contarNodosCondicion(&((*arbol)->der), condicion, cmp) + contarNodosCondicion(&((*arbol)->izq), condicion, cmp);

}

float promedioNodosClavePar(tArbol *arbol, int esPar(void *), void sumarClave(int *, void *)){
    int sumador = 0, cantidadNodos;
    float res;

    cantidadNodos = _promedioNodosClavePar(arbol, &sumador, esPar, sumarClave);
    res = (float) sumador / cantidadNodos;

    return res;
}

int _promedioNodosClavePar(tArbol *arbol, int *sumador, int esPar(void *), void sumarClave(int *, void *)){

    // Caso base
    if(!*arbol)
        return 0;

    if(esPar((*arbol)->info)){
        sumarClave(sumador, (*arbol)->info);
        return _promedioNodosClavePar(&((*arbol)->der), sumador, esPar, sumarClave) + _promedioNodosClavePar(&((*arbol)->izq), sumador, esPar, sumarClave) + 1;
    }
    else
        return _promedioNodosClavePar(&((*arbol)->der), sumador, esPar, sumarClave) + _promedioNodosClavePar(&((*arbol)->izq), sumador, esPar, sumarClave);
}

int saberAltura(tArbol *arbol){
    int alturaIzq, alturaDer;

    if(!*arbol)
        return 0;

    alturaIzq = saberAltura(&(*arbol)->izq);
    alturaDer = saberAltura(&(*arbol)->der);

    return MAX(alturaIzq, alturaDer) + 1;
}

void verHastaAltura(tArbol *arbol, int altura, void print(void *)){

    if(!*arbol || altura == 0)
        return;

    print((*arbol)->info);

    verHastaAltura(&(*arbol)->izq, altura - 1, print);
    verHastaAltura(&(*arbol)->der, altura - 1, print);
}

void verNodosUnaAltura(tArbol *arbol, int altura, void print(void *)){

    if(!*arbol)
        return;

    if(altura == 1){
        print((*arbol)->info);
        return;
    }

    verNodosUnaAltura(&(*arbol)->izq, altura - 1, print);
    verNodosUnaAltura(&(*arbol)->der, altura - 1, print);
}

void verNodosDesdeAltura(tArbol *arbol, int altura, void print(void *)){

    if(!*arbol)
        return;

    if(altura <= 1)
        print((*arbol)->info);

    verNodosDesdeAltura(&(*arbol)->izq, altura - 1, print);
    verNodosDesdeAltura(&(*arbol)->der, altura - 1, print);
}

int eliminarNodo(tArbol *arbol, void *info, unsigned tamInfo, int cmp(void *, void *)){

    if(!(arbol = buscarElemento(arbol, info, tamInfo, cmp)))
        return ELEMENTO_NO_ENCONTRADO;

    return eliminarRaiz(arbol);
}

int eliminarRaiz(tArbol *arbol){
    tArbol *reemp, elim;

    /// 1. Si el puntero esta vacio, retorno no encontrado
    if(!*arbol)
        return ELEMENTO_NO_ENCONTRADO;

    /// 2. Libero la informacion del nodo
    free((*arbol)->info);

    /// 3. Si el nodo a eliminar es una hoja, lo libero dirctamente y retorno EXITO
    if(!(*arbol)->izq && !(*arbol)->der){
        free(*arbol);
        *arbol = NULL;
        return EXITO;
    }

    /// 4. Si no es una hoja, busco el subarbol con mayor altura y tomo la direccion del puntero que apunta a dicho subarbol
    reemp = saberAltura(&(*arbol)->izq) > saberAltura(&(*arbol)->der) // --> Guarda la direccion del puntero que tiene la direccion del nodo derecho o izquierdo
            ? buscarNodoClaveMayor(&(*arbol)->izq) : buscarNodoClaveMenor(&(*arbol)->der);

    /// 5. Guardamos el nodo reemplazador para luego ser eliminado (ya que sus datos ya se pasarán al nodo raiz eliminado)
    elim = *reemp;

    /// 6. Copio los datos del nodo reemplazador al nodo con el contenido liberado (nodo a eliminar)
    memcpy((*arbol)->info, elim->info, (*arbol)->tamInfo);
    (*arbol)->tamInfo = elim->tamInfo;

    /// 7. Conecto los hijos del nodo reemplazador al nodo que apuntaba a dicho nodo reemplazador, si el nodo era el max de
    ///    la rama izquierda, solo puede tener nodos hijos por izquierda, y viceversa si el nodo era el min de la rama derecha
    *reemp = elim->izq ? elim->izq : elim->der;

    /// 8. Libero el nodo reemplazador
    free(elim);

    return EXITO;
}

int esArbolCompletoEnAltura(tArbol *arbol, int n){

    if(!*arbol)
        return 0;

    if(n == 1)
        return 1;

    return esArbolCompletoEnAltura(&(*arbol)->izq, n-1) && esArbolCompletoEnAltura(&(*arbol)->der, n-1);
}

int esArbolCompleto(tArbol *arbol){

    if(!arbol || !*arbol)
        return 1;

    return esArbolCompletoEnAltura(arbol, saberAltura(arbol));
}

int esArbolAVL(tArbol *arbol){
    int alturaIzq, alturaDer;

    if(!*arbol)
        return 1;

    alturaIzq = saberAltura(&(*arbol)->izq);
    alturaDer = saberAltura(&(*arbol)->der);

    if(ABS_DIFF(alturaIzq, alturaDer) > 1)
        return 0;

    // Si el nodo es AVL, seguimos con los hijos, en el caso de que
    // alguno de ellos no la pase, se genera un 0 y se propaga "hacia arriba"
    return esArbolAVL(&(*arbol)->izq) && esArbolAVL(&(*arbol)->der);
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
