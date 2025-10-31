#include "laberinto.h"

int laberinto()
{
    // VARIABLES
    int filas, cols, vidasInicio, cantFantasmas, cantPremios, cantVidasExtra, puntos = 0, ni, nj, fi, fj, contadorMovs = 0;
    char movInsertado, movSacado, **lab;

    tCola colaMov, colaMovHistorial, colaMovsFantasmas;

    Pos E, S, J, posRandom;
    Fantasma *fantasmas;
    MovFantasma movimientoFantasma;


    // LECTURA DE LOS DATOS DEL ARCHIVOS DE TEXTO PARA LA CREACION DEL LABERINTO
    if(leerDatosDeArchivo(&filas ,&cols , &vidasInicio, &cantFantasmas, &cantPremios, &cantVidasExtra) == ERROR_ARCHIVO)
    {
        printf("Error al leer archivo de texto");
        return ERROR_ARCHIVO;
    }
    // =============

    srand(time(NULL));

    // CREACION DEL LABERINTO Y POSICIONAMIENTO DE ENTRADA, SALIDA Y JUGADOR
    lab = creacionYPosicionamientoLab(&E, &S, &J, filas, cols);
    if(!lab)
    {
        printf("Error al tomar memoria para el laberinto");
        return ERROR_TOMAR_MEMORIA;
    }
    // =============

    // RESERVAMOS LUGAR PARA LA MATRIZ CON LA POSICION DE LOS FANTASMAS
    fantasmas = malloc(cantFantasmas * sizeof(Fantasma));
    // =============

    // COLOCAMOS LA VIDA, LOS PREMIOS Y LOS FANTASMAS
    for(int i = 0; i < cantVidasExtra; i++)
    {
        do
        {
            posRandom = posRandomVidasyPuntos(filas, cols);

        } while(lab[posRandom.i][posRandom.j] != '.');

        lab[posRandom.i][posRandom.j] = 'V';
    }

    for(int i = 0; i < cantFantasmas; i++)
    {

        do
        {
            posRandom = posRandomVidasyPuntos(filas, cols);
        } while(lab[posRandom.i][posRandom.j] != '.');

        lab[posRandom.i][posRandom.j] = 'F';

        fantasmas[i].id = (i);
        fantasmas[i].posicion.i = posRandom.i;
        fantasmas[i].posicion.j = posRandom.j;
        fantasmas[i].estaVivo = 1;
        fantasmas[i].debajo = '.';
    }

    for(int i = 0; i < cantPremios; i++)
    {
        do
        {
            posRandom = posRandomVidasyPuntos(filas, cols);

        } while(lab[posRandom.i][posRandom.j] != '.');

        lab[posRandom.i][posRandom.j] = 'P';
    }
    // =============


    // CREAMOS LAS COLAS DE LOS MOVIMIENTOS
    crearCola(&colaMov);
    crearCola(&colaMovHistorial);
    crearCola(&colaMovsFantasmas);
    // =============


    /// LOOP DE JUEGO
    while(vidasInicio > 0)
    {

        // MOSTRAMOS EL TABLERO
        system("cls"); // o "clear" en Linux
        printf("\n");
        for(int i=0;i<filas;i++)
        {
            printf("\t");
            for(int j=0;j<cols;j++)
                printf("%c ", lab[i][j]);
            printf("\n");
        }
        // =============

        // COMPROBAMOS SI SE GANO LA PARTIDA
        if(J.i == S.i && J.j == S.j)
        {
            system("cls");
            printf("\nLlegaste a la salida!!\n");

            printf("\n\nHistorial de movimientos: ");

            while(sacarDeCola(&colaMovHistorial, &movSacado, sizeof(movSacado)) ==  EXITO)
            {
                contadorMovs++;
                printf("%c", movSacado);

                if(colaVacia(&colaMovHistorial) != COLA_VACIA)
                    printf("-");
            }


            printf("\nTotal de movimientos: %d", contadorMovs);
            /*
            printf("\nHistorial de movimientos de los fantasmas:\n");
            while(sacarDeCola(&colaMovsFantasmas, &m, sizeof(m)) ==  EXITO) {
                printf(" %d%c -", m.idFantasma, m.mov);
            }
            */

            printf("\n\n\n");
            system("pause");

            return DERROTA;
        }
        //=============

        // INDICAMOS LOS PUNTOS Y LAS VIDAS
        printf("\n");
        printf("\tVidas: %d\tPuntos: %d", vidasInicio, puntos);
        printf("\n");
        // =============

        // TOMAMOS EL PROXIMO MOVIMIENTO DEL JUGADOR Y LO ENCOLAMOS

        printf("\nMover (w=arriba, s=abajo, a=izq, d=der): ");
        movInsertado = tolower(getch());
        while (movInsertado != 'w' && movInsertado != 'a' && movInsertado != 's' && movInsertado != 'd')
        {
            printf("\n\nInserte un movimiento valido\n");
            printf("Mover (w=arriba, s=abajo, a=izq, d=der): ");
            movInsertado = tolower(getch());
        }
        ponerEnCola(&colaMov, &movInsertado, sizeof(movInsertado));
        // =============

        // CALCULAMOS EL MOVIMIENTO DEL FANTASMA Y LO ENCOLAMOS
        for(int f = 0; f < cantFantasmas; f++)
        {

            if(fantasmas[f].estaVivo)
            {

                movimientoFantasma.id = fantasmas[f].id;

                fi = fantasmas[f].posicion.i;
                fj = fantasmas[f].posicion.j;

                int difI = J.i - fi;
                int difJ = J.j - fj;

                if(abs(difI) > abs(difJ))
                {
                    // mover en vertical
                    if(difI >= 0)
                        movimientoFantasma.proxMov = 's'; // bajar
                    else if(difI <= 0)
                        movimientoFantasma.proxMov = 'w'; // subir
                } else
                {
                    // mover en horizontal
                    if(difJ >= 0)
                        movimientoFantasma.proxMov = 'd'; // derecha
                    else if(difJ <= 0)
                        movimientoFantasma.proxMov = 'a'; // izquierda
                }

                ponerEnCola(&colaMovsFantasmas, &movimientoFantasma, sizeof(movimientoFantasma));
            }
        }

        // TOMAMOS EL MOVIMIENTO DEL JUGADOR DE LA COLA Y LO REFLEJAMOS EN EL LAB
        while(sacarDeCola(&colaMov, &movSacado, sizeof(movSacado)) == EXITO)
        {

            // COPIAMOS LA POSICION ACTUAL DEL JUGADOR
            ni = J.i;
            nj = J.j;
            // =============

            // HACEMOS LA MODIFICACION DE LA POSICION SOBRE LA COPIA
            if(movSacado == 'w') ni--;
            else if(movSacado == 's') ni++;
            else if(movSacado == 'a') nj--;
            else if(movSacado == 'd') nj++;
            // =============

            // SI EL MOV ES VALIDO, DEJAMOS EL RASTRO Y LO CARGAMOS EN LA COLA DE MOV HISTORICOS
            if(lab[ni][nj] != '#' && lab[ni][nj] != 'E')
            {
                lab[J.i][J.j] = (J.i == E.i && J.j == E.j) ? 'E' : '.'; // dejar rastro

                ponerEnCola(&colaMovHistorial, &movSacado, sizeof(movSacado));
            }
            // =============

            // PARA CADA TIPO DE OBJETO QUE NOS CRUCEMOS HACEMOS UNA ACCION
            if(lab[ni][nj] == '.' || lab[ni][nj] == 'S')
            {
                J.i = ni;
                J.j = nj;
                lab[J.i][J.j] = 'J';
            }

            if(lab[ni][nj] == 'F')
            {

                for(int i = 0;i < cantFantasmas;i++)
                {
                    if(fantasmas[i].estaVivo && fantasmas[i].posicion.i == ni && fantasmas[i].posicion.j == nj)
                        fantasmas[i].estaVivo = 0;
                }

                vidasInicio--;
                lab[ni][nj] = (ni == E.i && nj == E.j) ? 'E' : (ni == S.i && nj == S.j) ? 'S' : '.';
                lab[J.i][J.j] = (J.i == E.i && J.j == E.j) ? 'E' : (J.i == S.i && J.j == S.j) ? 'S' : '.';
                // Colocar jugador en la entrada
                J.i = E.i + 1;
                J.j = E.j;
                lab[J.i][J.j] = 'J';
            }

            if(lab[ni][nj] == 'V')
            {
                vidasInicio++;
                // Colocar jugador en la entrada
                J.i = ni;
                J.j = nj;
                lab[J.i][J.j] = 'J';
            }

            if(lab[ni][nj] == 'P')
            {
                puntos++;
                // Colocar jugador en la entrada
                J.i = ni;
                J.j = nj;
                lab[J.i][J.j] = 'J';
            }
            // =============

        }
        // =============

        // TOMAMOS LOS MOVIMIENTOS DEL FANTASMA DE LA COLA Y LOS REFLEJAMOS EN EL LAB
        while(sacarDeCola(&colaMovsFantasmas, &movimientoFantasma, sizeof(movimientoFantasma)) == EXITO)
        {

            int i = movimientoFantasma.id;
            if(fantasmas[i].estaVivo){

                ni = fantasmas[i].posicion.i;
                nj = fantasmas[i].posicion.j;

                fi = ni;
                fj = nj;

                if(movimientoFantasma.proxMov == 'w') ni--;
                else if(movimientoFantasma.proxMov == 's') ni++;
                else if(movimientoFantasma.proxMov == 'a') nj--;
                else if(movimientoFantasma.proxMov == 'd') nj++;

                if(lab[ni][nj] != '#' && lab[ni][nj] != 'F')
                {

                    lab[fi][fj] = (fi == E.i && fj == E.j) ? 'E' : (fi == S.i && fj == S.j) ? 'S' : '.';

                    if(lab[ni][nj] == 'J')
                    {

                        fantasmas[i].estaVivo = 0;

                        vidasInicio--;

                        lab[J.i][J.j] = '.';
                        J.i = E.i + 1;
                        J.j = E.j;
                        lab[J.i][J.j] = 'J';

                        vaciarCola(&colaMovsFantasmas);

                    } else
                    {
                        lab[fi][fj] = fantasmas[i].debajo;

                        fantasmas[i].debajo = lab[ni][nj];

                        fantasmas[i].posicion.i = ni;
                        fantasmas[i].posicion.j = nj;
                        lab[ni][nj] = 'F';
                    }

                }

            }

        }

    }
    //=============

    // SI NO TENEMOS MAS VIDAS PERDEMOS
    if(!vidasInicio)
        mostrarGameOver();
    // =============

    // LIBERAMOS EL LABERINTO
    for(int i=0;i<filas;i++)
        free(lab[i]);
    free(lab);
    // =============

    return EXITO;
}

int leerDatosDeArchivo(int *filas, int *cols, int *vidasInicio, int *cantFantasmas, int *cantPremios, int *cantVidasExtra)
{
    FILE *f;

    f = fopen("config.txt", "rt");
    if(!f)
        return ERROR_ARCHIVO;

    fscanf(f,"filas: %d\ncolumnas: %d\nvidas_inicio: %d\nmaximo_numero_fantasmas: %d\nmaximo_numero_premios: %d\nmaximo_vidas_extra: %d",
           filas , cols, vidasInicio, cantFantasmas, cantPremios, cantVidasExtra);

    fclose(f);

    return EXITO;
}


char ** creacionYPosicionamientoLab(Pos *E, Pos *S, Pos *J, int filas, int cols)
{

    // RESERVAR MATRIZ PARA LAB DE JUEGO
    char **lab = malloc(filas * sizeof(char*));
    if(!lab)
        return NULL;


    for(int i=0;i<filas;i++)
    {
        lab[i] = malloc(cols * sizeof(char));
        if(!lab[i]) {
            // LIBERA LO YA RESERVADO
            for(int k=0; k<i; k++) free(lab[k]);
            free(lab);
            // =============
            return NULL;
        }
    }
    // =============

    // BUCLE PARA OBTENER UN LABERINTO JUGABLE
    do
    {
        // RELLENAMOS ALEATORIAMENTE UN LABERINTO
        for(int i=0;i<filas;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(i==0||i==filas-1||j==0||j==cols-1)
                    lab[i][j] = '#'; // bordes paredes
                else
                    lab[i][j] = (rand()%100 < 30)? '#' : '.'; // 30% paredes
            }
        }

        // COLOCAMOS LA ENTRADA Y SALIDA
        E->i = 0;
        E->j = 1;
        S->i = filas-1;
        S->j = cols-2;
        lab[E->i][E->j] = 'E';
        lab[S->i][S->j] = 'S';
        // =============

    } while(!bfs(lab, filas, cols, (*E), (*S)));
    // =============

    // COLOCAMOS AL JUGADOR EN LA ENTRADA
    J->i = E->i + 1;
    J->j = E->j;
    lab[J->i][J->j] = 'J';
    // =============

    return lab;
}

// BFS para ver si hay camino de ini a fin
int bfs(char **lab, int filas, int cols, Pos ini, Pos fin)
{
    tCola cola;
    Pos act;
    int **visitado = malloc(filas * sizeof(int*));

    for(int i=0;i<filas;i++)
        visitado[i] = calloc(cols, sizeof(int));

    crearCola(&cola);
    ponerEnCola(&cola, &ini, sizeof(ini));
    visitado[ini.i][ini.j] = 1;

    int di[4] = {-1,1,0,0};
    int dj[4] = {0,0,-1,1};

    while(colaVacia(&cola) == ESPACIO_DISPONIBLE)
    {
        sacarDeCola(&cola, &act, sizeof(act));
        if(act.i == fin.i && act.j == fin.j)
        {
            // liberar visitado
            for(int i=0;i<filas;i++)
                free(visitado[i]);

            free(visitado);
            return 1; // encontrado
        }

        for(int k=0;k<4;k++)
        {
            int ni = act.i + di[k];
            int nj = act.j + dj[k];

            if(0 <= ni && ni < filas && 0 <= nj && nj < cols && !visitado[ni][nj] && lab[ni][nj] != '#')
            {
                Pos vecino = {ni,nj};
                ponerEnCola(&cola, &vecino, sizeof(vecino));
                visitado[ni][nj] = 1;
            }
        }
    }

    // liberar visitado
    for(int i=0;i<filas;i++)
        free(visitado[i]);
    free(visitado);
    return 0; // no encontrado
}

Pos posRandomVidasyPuntos(int fil, int col)
{
    Pos posicion;

    posicion.i = rand()%(fil-2) + 1;
    posicion.j = rand()%(col-2) + 1;

    return posicion;
}

void mostrarGameOver()
{
    system("cls");

    printf("\n\n\n");
    printf("\t#       ###    ####   #### \n");
    printf("\t#      #   #  #      #     \n");
    printf("\t#      #   #   ###    ###  \n");
    printf("\t#      #   #      #      # \n");
    printf("\t#####   ###   ####   ####  \n\n\n");

    system("pause");
}

