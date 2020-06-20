/*
PRACTICO DE MAQUINA SISTEMAS DISTRIBUIDOS Y PARALELOS 2020.
ALUMNO: FEDERICO A. VERGES

Para Ejecutar: "gcc -o <nombre-ejecutable> <nombre-archivo.c>", luego "./ejecutable"

*/
#include <stdio.h>
#include <stdlib.h>
#include "Celdas.h"
#include <sys/time.h>

// Dimension de la matriz
#define FILAS 198
#define COLUMNAS 198
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

    for (int i = 0; i < cant_Vecinos; i += 1)
    { // Se suma dos por el arreglo de vecinos.
        int cordx_vecino = arr_vecino[i].cord_x;
        int cordy_vecino = arr_vecino[i].cord_y;

        if (cordx_vecino == 0 || cordy_vecino == 0 || cordx_vecino == frontera_fila - 1 || cordy_vecino == frontera_columna - 1)
        {
        }
        else
        {
            vecinos++;
            vecino = tablero[cordx_vecino][cordy_vecino];
            // printf("Vecinos de %d %d", c.cord_x, c.cord_y);
            // mostrarColores(vecino);
            // printf("\n");
            if (vecino.estado == ROJO)
            {
                vecinos_infectados += 1;
            }
        }
    }
    printf("Cantidad de vecinos en la celda: %d %d contagiados =  %d\n", c.cord_x, c.cord_y, vecinos_infectados);
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
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
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
            if (i == 0 || j == 0 || i == filas - 1 || j == columnas - 1)
            {
                edad = -1;
                color = CELESTE;
                tablero[i][j].cord_x = i;
                tablero[i][j].cord_y = j;
                tablero[i][j].edad = edad;
                tablero[i][j].estado = color;
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
                // if (random_colores > 80 && random_colores <= 90)
                // {
                //     color = NARANJA;
                //     init_timer = 1;
                // // }
                // if (random_colores > 90 && random_colores <= 100)
                // {
                //     color = AMARILLO;
                //     init_timer = 7;
                // }

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
                tablero[i][j].cord_x = i;
                tablero[i][j].cord_y = j;
                tablero[i][j].edad = edad;
                tablero[i][j].estado = color;

                tablero[i][j].grupo_riesgo = grupo_riesgo;
                tablero[i][j].sexo_biologico = sexo_biologico;
                tablero[i][j].timer = init_timer;
                tablero[i][j].vacunas = vacunas;
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
    int datos_vecinos[2]; // Arreglo con la cantida de vecinos y la cantidad de infectados.
    //Evaluar estado de celda.
    if ((c).estado == AZUL)
    {
        // Suceptible -> Enfermo sin contagio.
        //getCeldasInfectadas(c, tablero, VECINOS, frontera_fila, frontera_columna, &datos_vecinos);

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
        }
    }
    else if ((c).estado == NARANJA)
    {
        // enfermo Sin.C -> enfermo con C.
        if ((c).timer == 4)
        {
            c.estado = ROJO;
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
            }
        }
        // Muerto || Recuperado
        if ((c).timer == 14)
        {
            mortalidad = EvaluarMortalidad(c);

            if (drand48() <= mortalidad)
            {
                c.estado = NEGRO;
            }
            else
            {
                c.estado = VERDE;
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
            }
            else
            {
                c.estado = VERDE;
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
    Celda **tablero_auxiliar;
    tablero_auxiliar = (Celda **)malloc(sizeof(Celda *) * frontera_filas);
    if (tablero_auxiliar == NULL)
    {
    }
    for (int i = 0; i < frontera_filas; i++)
    {
        tablero_auxiliar[i] = (Celda *)malloc(sizeof(Celda) * frontera_columnas);
        if (tablero_auxiliar[i] == NULL)
        {
        }
    }

    // Inicializando matriz.
    initMatrix(frontera_filas, frontera_columnas, tablero);

    // Celda de prueba.
    Celda prueba;
    prueba.cord_x = 2;
    prueba.cord_y = 2;
    prueba.edad = 50;
    prueba.estado = ROJO;
    prueba.grupo_riesgo = 0;
    prueba.sexo_biologico = 1;
    prueba.timer = 13;
    prueba.vacunas = 1;
    //tablero[2][2] = prueba;

    //Copiando tablero a tablero_auxiliar
    for (int i = 0; i < frontera_filas; i++)
    {
        for (int j = 0; j < frontera_columnas; j++)
        {
            tablero_auxiliar[i][j] = tablero[i][j];
        }
    }

    Celda **temp; // puntero temporal para hacer swap

    // SIMULACION //
    struct timeval ti, tf;
    double tiempo;
    double tiempo_total = 0;
    double tiempo_promedio = 0;

    for (int i = 0; i < LAPS; i++)
    {
        // Inicializando matriz.
        initMatrix(frontera_filas, frontera_columnas, tablero);

        gettimeofday(&ti, NULL); // Tiempo inicial
        for (int i = 0; i < DIAS; i++)
        {
            // Actualizando proximos estados de la matriz.
            for (int i = 1; i < frontera_filas - 1; i++)
            {
                for (int j = 1; j < frontera_columnas - 1; j++)
                {
                    Calcular_Proximo_Estado(tablero, tablero_auxiliar, tablero[i][j], frontera_filas, frontera_columnas);
                }
            }

            // Swap de punteros de la matriz.
            temp = tablero;
            tablero = tablero_auxiliar;
            tablero_auxiliar = temp;

            // // Mostramos los valores de la matriz.
            // printf("\n Tablero \n");
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

            // // Mostramos datos relevantes de las celdas.
            // for (int i = 1; i < frontera_filas - 1; i++)
            // {
            //     for (int j = 1; j < frontera_columnas - 1; j++)
            //     {
            //         printf("\n Celda:");
            //         printf("%d ", tablero[i][j].cord_x);
            //         printf("%d ", tablero[i][j].cord_y);
            //         // printf("%d", tablero[i][j].edad);
            //         mostrarColores(tablero[i][j]);

            //         // printf("%d", tablero[i][j].grupo_riesgo);
            //         // printf("%d", tablero[i][j].sexo_biologico);
            //         printf("%d ", tablero[i][j].timer);
            //         // printf("%d    ", tablero[i][j].vacunas);
            //     }
            //     printf("\n");
            // }
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