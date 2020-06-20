/*
PRACTICO DE MAQUINA SISTEMAS DISTRIBUIDOS Y PARALELOS 2020.
ALUMNO: FEDERICO A. VERGES

Para Ejecutar: "gcc -o <nombre-ejecutable> -lm -fopenmp <nombre-archivo.c>", luego "./ejecutable"
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Celdas.h"
#include <omp.h>
#include <sys/time.h>

// Dimension de la matriz
#define FILAS 10
#define COLUMNAS 10
#define LAPS 10

// Simulacion
#define DIAS 120

// Funcion para mostrar los colores de las celdas.
void mostrarColores(Celda c)
{
    if (c.estado == BLANCO)
    {
        printf(" Blanco ");
    }
    if (c.estado == AZUL)
    {
        printf(" AZUL ");
    }
    if (c.estado == ROJO)
    {
        printf(" ROJO ");
    }
    if (c.estado == VERDE)
    {
        printf(" VERDE ");
    }
    if (c.estado == NARANJA)
    {
        printf(" NARANJA ");
    }
    if (c.estado == AMARILLO)
    {
        printf(" AMARILLO ");
    }
    if (c.estado == NEGRO)
    {
        printf(" NEGRO ");
    }
    if (c.estado == CELESTE)
    {
        printf(" F ");
    }
}

void getCeldasInfectadas(Celda c, Celda **tablero, int cant_Vecinos, int frontera_fila, int frontera_columna, int *datos_vecino)
{
    int vecinos_infectados = 0;
    int vecinos = 0;
    Celda vecino;

    Celda arr_vecino[8] = {
        tablero[c.cord_x + ARRIBA][c.cord_y + IZQUIERDA],
        tablero[c.cord_x + ARRIBA][c.cord_y + CENTRO],
        tablero[c.cord_x + ARRIBA][c.cord_y + DERECHA],
        tablero[c.cord_x + CENTRO][c.cord_y + IZQUIERDA],
        tablero[c.cord_x + CENTRO][c.cord_y + DERECHA],
        tablero[c.cord_x + ABAJO][c.cord_y + IZQUIERDA],
        tablero[c.cord_x + ABAJO][c.cord_y + CENTRO],
        tablero[c.cord_x + ABAJO][c.cord_y + DERECHA]};

    for (int i = 0; i < cant_Vecinos; i += 2)
    { // Se suma dos por el arreglo de vecinos.
        int cordx_vecino = arr_vecino[i].cord_x;
        int cordy_vecino = arr_vecino[i].cord_y;

        if (cordx_vecino == 0 || cordy_vecino == 0 || cordx_vecino == frontera_fila || cordy_vecino == frontera_columna)
        {
        }
        else
        {
            vecinos++;
            vecino = tablero[cordx_vecino][cordy_vecino];
            //mostrarColores(vecino);
            if (vecino.estado == ROJO)
            {
                vecinos_infectados += 1;
            }
        }
    }
    // printf("Cantidad de vecinos en la celda: %d %d contagiados =  %d\n", c.cord_x, c.cord_y, vecinos_infectados);
    datos_vecino[0] = vecinos;
    datos_vecino[1] = vecinos_infectados;
}

float EvaluarMortalidad(Celda c)
{
    float mortalidad;
    if (c.edad < 22)
    {
        mortalidad = 0.01;
    }
    else if (c.edad >= 22 && c.edad <= 64)
    {
        mortalidad = 0.013;
    }
    else if (c.edad > 64)
    {
        mortalidad = 0.148;
    }

    if (c.vacunas == 1)
    {
        mortalidad -= 0.005;
    }
    return mortalidad;
}

void initMatrix(int filas, int columnas, Celda **tablero)
{
    srand(time(NULL));
    int indice_fila = 0;
    int indice_columna = 0;

    for (indice_fila = 0; indice_fila < filas; indice_fila++)
    {
        for (indice_columna = 0; indice_columna < columnas; indice_columna++)
        {
            int random_edad = rand() % 101;
            int random_colores = rand() % 101; // Densidad Poblacional
            int color = -1;
            int edad;
            int init_timer = 0;
            int grupo_riesgo = 0;
            int sexo_biologico = 0;
            int vacunas = 0;
            //Frontera

            if (indice_fila == 0 || indice_columna == 0 || indice_fila == filas - 1 || indice_columna == columnas - 1)
            {
                edad = -1;
                color = CELESTE;
                tablero[indice_fila][indice_columna].cord_x = indice_fila;
                tablero[indice_fila][indice_columna].cord_y = indice_columna;
                tablero[indice_fila][indice_columna].edad = edad;
                tablero[indice_fila][indice_columna].estado = color;
            }
            else
            {

                if ((rand() % 3) == 2)
                {
                    grupo_riesgo = 1;
                }
                if ((rand() % 2) == 1)
                {
                    sexo_biologico = 1;
                }
                if ((rand() % 3) < 2)
                {
                    vacunas = 1;
                }
                // Color
                if (random_colores < 50)
                {
                    color = BLANCO;
                }
                if (random_colores >= 50 && random_colores < 99)
                {
                    color = AZUL;
                }
                if (random_colores >= 99 && random_colores <= 100)
                {
                    color = ROJO;
                    init_timer = 5;
                }
                // Random Edad
                if (random_edad < 30)
                {
                    edad = rand() % 21;
                }
                else if (random_edad >= 30 && random_edad < 85)
                {
                    edad = 54;
                }
                else
                {
                    edad = 90;
                }
                // Crear celda.
                tablero[indice_fila][indice_columna].cord_x = indice_fila;
                tablero[indice_fila][indice_columna].cord_y = indice_columna;
                tablero[indice_fila][indice_columna].edad = edad;
                tablero[indice_fila][indice_columna].estado = color;

                tablero[indice_fila][indice_columna].grupo_riesgo = grupo_riesgo;
                tablero[indice_fila][indice_columna].sexo_biologico = sexo_biologico;
                tablero[indice_fila][indice_columna].timer = init_timer;
                tablero[indice_fila][indice_columna].vacunas = vacunas;
            }
        }
    }
}

void Calcular_Proximo_Estado(Celda **tablero, Celda **tablero_auxiliar, Celda c, int frontera_fila, int frontera_columna)
{
    srand(time(NULL));
    srand48(time(NULL));
    int prob_aislamiento = 0;
    float mortalidad = 0;
    float prob_contagio = 0;
    int *datos_vecinos = malloc(sizeof(int) * 2); // Arreglo con la cantida de vecinos y la cantidad de infectados.

    //Evaluar estado de celda.
    if ((c).estado == AZUL)
    {
        // Suceptible -> Enfermo sin contagio.
        getCeldasInfectadas(c, tablero, VECINOS, frontera_fila, frontera_columna, datos_vecinos);

        if (datos_vecinos[1] == 0)
        {
            prob_contagio = 0;
        }
        else
        {
            prob_contagio = Suceptible_Enf_sin_contagio(c, datos_vecinos[0], datos_vecinos[1]);
        }

        if (drand48() <= prob_contagio)
        {
            c.estado = NARANJA;
            //printf("**\n La celda %d %d se ha contagiado **\n", (c).cord_x, (c).cord_y);
        }
    }
    else if ((c).estado == NARANJA)
    {
        // enfermo Sin.C -> enfermo con C.
        if ((c).timer == 4)
        {
            c.estado = ROJO;
            //printf("**\n  La celda %d %d está empenzando a contagiar **\n", (c).cord_x, (c).cord_y);
        }
    }
    else if ((c).estado == ROJO)
    {
        // Aislamiento
        if ((c).timer == 7)
        {
            prob_aislamiento = rand() % 101;
            if (prob_aislamiento < 90)
            {
                c.estado = AMARILLO;
                //printf("**\n  La celda %d %d está en aislamiento **\n", (c).cord_x, (c).cord_y);
            }
        }
        // Muerto || Recuperado
        if ((c).timer == 14)
        {
            mortalidad = EvaluarMortalidad(c);

            if (drand48() <= mortalidad)
            {
                c.estado = NEGRO;
                //printf("**\n  La celda %d %d ha muerto **\n", (c).cord_x, (c).cord_y);
            }
            else
            {
                //Actualizar_Estado(VERDE, tablero, c);
                c.estado = VERDE;
                //printf("**\n  La celda %d %d se ha recuperado ** \n", (c).cord_x, (c).cord_y);
            }
        }
    }
    else if ((c).estado == AMARILLO)
    {
        // Muerto || Recuperado
        if ((c).timer == 14)
        {
            mortalidad = EvaluarMortalidad(c);

            if (drand48() <= mortalidad)
            {
                c.estado = NEGRO;
                //printf("**\n  La celda %d %d ha muerto **\n", (c).cord_x, (c).cord_y);
            }
            else
            {
                c.estado = VERDE;
                //printf("**\n  La celda %d %d se ha recuperado ** \n", (c).cord_x, (c).cord_y);
            }
        }
    }
    else
    {
        // Estado ,blanco,negro y verde.
    }
    c.timer++;
    tablero_auxiliar[(c).cord_x][(c).cord_y] = c;
}

int main()
{

    //Frontera
    int frontera_filas = FILAS + 2;
    int frontera_columnas = COLUMNAS + 2;

    //Rerservando memoria para la matriz Original.
    Celda **tablero;
    Celda **tablero_auxiliar;
    Celda **temp; // puntero temporal para hacer swap
    tablero = (Celda **)malloc(sizeof(Celda *) * frontera_filas);
    if (tablero == NULL)
    {
        printf("No se ha podido reservar memoria");
    }

    for (int i = 0; i < frontera_filas; i++)
    {
        tablero[i] = (Celda *)malloc(sizeof(Celda) * frontera_columnas);
        if (tablero[i] == NULL)
        {
            printf("No se ha podido reservar memoria");
        }
    }

    //Rerservando memoria para la matriz Auxiliar (preserva los estados anteriores).
    tablero_auxiliar = (Celda **)malloc(sizeof(Celda *) * frontera_filas);
    if (tablero_auxiliar == NULL)
    {
        printf("No se ha podido reservar memoria");
    }

    for (int i = 0; i < frontera_filas; i++)
    {
        tablero_auxiliar[i] = (Celda *)malloc(sizeof(Celda) * frontera_columnas);
        if (tablero_auxiliar[i] == NULL)
        {
            printf("No se ha podido reservar memoria");
        }
    }
    printf("Antes de inicializar\n");

    // Inicializando matriz.
    initMatrix(frontera_filas, frontera_columnas, tablero);

    printf("Despues de inicializar\n");

    // Celda de prueba.
    Celda prueba;
    int vecinos_temp[16] = {ARRIBA, IZQUIERDA, ARRIBA, CENTRO, ARRIBA, DERECHA, CENTRO, IZQUIERDA, CENTRO, DERECHA, ABAJO, IZQUIERDA, ABAJO, CENTRO, ABAJO, DERECHA};
    prueba.cord_x = 2;
    prueba.cord_y = 2;
    prueba.edad = 50;
    prueba.estado = ROJO;
    prueba.grupo_riesgo = 0;
    prueba.sexo_biologico = 1;
    prueba.timer = 13;
    prueba.vacunas = 1;

    // tablero[2][2] = prueba;

    // for (int i = 0; i < frontera_filas; i++)
    // {
    //     for (int j = 0; j < frontera_columnas; j++)
    //     {
    //         // printf("%d", tablero[i][j].cord_x);
    //         // printf("%d", tablero[i][j].cord_y);
    //         // printf("%d", tablero[i][j].edad);
    //         mostrarColores(tablero[i][j]);
    //         // printf("%d", tablero[i][j].grupo_riesgo);
    //         // printf("%d", tablero[i][j].sexo_biologico);
    //         // printf("%d", tablero[i][j].timer);
    //         // printf("%d    ", tablero[i][j].vacunas);
    //     }
    //     printf("\n");
    // }

    // Paralelismo //
    int num_process;
    omp_set_num_threads(4);

    // Simulación

    struct timeval ti, tf;
    double tiempo;
    double tiempo_total = 0;
    double tiempo_promedio = 0;
    for (int i = 0; i < LAPS; i++)
    {
        gettimeofday(&ti, NULL); // Tiempo inicial
        for (int i = 0; i < DIAS; i++)
        {
// Actualizando proximos estados de la matriz.
#pragma omp parallel private(num_process)
            {
                num_process = omp_get_thread_num();
#pragma omp for
                for (int indice_fila = 0; indice_fila < frontera_filas; indice_fila++)
                {
                    for (int indice_columna = 0; indice_columna < frontera_columnas; indice_columna++)
                    {
                        Calcular_Proximo_Estado(tablero, tablero_auxiliar, tablero[indice_fila][indice_columna], frontera_filas, frontera_columnas);
                    }
                }
            }

            // Swap de punteros de la matriz.
            temp = tablero;
            tablero = tablero_auxiliar;
            tablero_auxiliar = temp;

          
        }
        gettimeofday(&tf, NULL);
        tiempo = (tf.tv_sec - ti.tv_sec) * 1000.0 + (tf.tv_usec - ti.tv_usec) / 1000.0;
        printf("Has tardado: %g milisegundos\n", tiempo);
        tiempo_total += tiempo;
        tiempo = 0;
    }
    tiempo_promedio = tiempo_total / LAPS;

    printf("Tiempo Promedio en 10 corridas: %g milisegundos \n", tiempo_promedio);

    return 0;
}