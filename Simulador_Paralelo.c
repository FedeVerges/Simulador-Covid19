/*
PRACTICO DE MAQUINA SISTEMAS DISTRIBUIDOS Y PARALELOS 2020.
ALUMNO: FEDERICO A. VERGES
Para Ejecutar: "mpicc -o <nombre-ejecutable>  <nombre-archivo.c>", luego "mpirun -np <numero de proceos> <archivo.c"
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Celdas.h"
#include <sys/time.h>

// Dimension de la matriz
#define FILAS 200
#define COLUMNAS 200
#define LAPS 10

// Simulacion
#define DIAS 15

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

void initMatrix(int filas, int columnas, Celda *tablero)
{
    srand(time(NULL));
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            int k = (i * columnas) + j; // k = (index de la fila * cantidad de columnas) + indice de la columna.  Cantidad de columnas= largo de la fila.
            int random_edad = rand() % 101;
            int random_colores = rand() % 101;
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
                tablero[k].cord_x = i;
                tablero[k].cord_y = j;
                tablero[k].edad = edad;
                tablero[k].estado = color;
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
                tablero[k].cord_x = i;
                tablero[k].cord_y = j;
                tablero[k].edad = edad;
                tablero[k].estado = color;

                tablero[k].grupo_riesgo = grupo_riesgo;
                tablero[k].sexo_biologico = sexo_biologico;
                tablero[k].timer = init_timer;
                tablero[k].vacunas = vacunas;
            }
        }
    }
}

void getCeldasInfectadas(Celda c, Celda *buffer_fila, Celda *buffer_fila_sup, Celda *buffer_fila_inf, int cant_Vecinos, int frontera_fila, int frontera_columna, int *datos_vecino)
{
    int vecinos_infectados = 0;
    int vecinos = 0;

    Celda vecino[8] = {
        buffer_fila_sup[c.cord_y - 1],
        buffer_fila_sup[c.cord_y],
        buffer_fila_sup[c.cord_y + 1],
        buffer_fila[c.cord_y - 1],
        buffer_fila[c.cord_y + 1],
        buffer_fila_inf[c.cord_y - 1],
        buffer_fila_inf[c.cord_y],
        buffer_fila_inf[c.cord_y + 1]};

    for (int i = 0; i < cant_Vecinos; i += 1)
    { // Se suma dos por el arreglo de vecinos.

        if (vecino[i].cord_y == 0 || vecino[i].cord_y == 0 || vecino[i].cord_y == frontera_fila || vecino[i].cord_y == frontera_columna)
        {
        }
        else
        {
            vecinos++;
            //mostrarColores(vecino);
            if (vecino[i].estado == ROJO)
            {
                vecinos_infectados += 1;
            }
        }
    }
    //printf("Cantidad de vecinos en la celda: %d %d contagiados =  %d\n", c.cord_x, c.cord_y, vecinos_infectados);
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

void Calcular_Proximo_Estado(Celda *tablero, Celda *buffer_resultado, Celda c, Celda *buffer_fila, Celda *buffer_fila_sup, Celda *buffer_fila_inf, int frontera_fila, int frontera_columna)
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
        getCeldasInfectadas(c, buffer_fila, buffer_fila_sup, buffer_fila_inf, VECINOS, frontera_fila, frontera_columna, &datos_vecinos);

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
    }
    c.timer++;
    buffer_resultado[(c).cord_y] = c;
}

void getSubMatrix(Celda *matrix, Celda *submatrix, int start, int finish)
{
    // get the matrix part to send.
    for (int i = start; i < finish; i += 1)
    {
        submatrix[i - start] = matrix[i];
        // //printf("%d ", i);
    }
}

int main(int argc, char **argv)
{
    //Frontera
    int frontera_filas = FILAS + 2;
    int frontera_columnas = COLUMNAS + 2;
    int start; // Flag para indicar el desplazamiento.
    int retorno = 0;

    //Rerservando memoria para la matriz Original.
    Celda *tablero;
    Celda *tablero_auxiliar;
    Celda *temp;

    /* Paralelismo */
    Celda *buffer_fila_superior;
    Celda *buffer_fila_inferior;
    Celda *buffer_fila;
    Celda *submatrix;
    Celda *buffer_resultado;
    Celda *buffer_resultado_auxiliar;

    // Tablero Original
    tablero = (Celda *)malloc((frontera_filas * frontera_columnas) * sizeof(Celda));
    if (tablero == NULL)
    {
        printf("No se ha podido reservar memoria");
    }
    // Reservando memoria para los buffers.
    buffer_fila = (Celda *)malloc(frontera_columnas * sizeof(Celda));
    if (buffer_fila == NULL)
    {
        printf("No se ha podido reservar memoria");
    }
    buffer_fila_inferior = (Celda *)malloc(frontera_columnas * sizeof(Celda));
    if (buffer_fila_inferior == NULL)
    {
        printf("No se ha podido reservar memoria");
    }
    buffer_fila_superior = (Celda *)malloc(frontera_columnas * sizeof(Celda));
    if (buffer_fila_superior == NULL)
    {
        printf("No se ha podido reservar memoria");
    }

    buffer_resultado = (Celda *)malloc(frontera_columnas * sizeof(Celda));
    if (buffer_resultado == NULL)
    {
        printf("No se ha podido reservar memoria");
    }

    MPI_Init(&argc, &argv);
    MPI_Status status;

    //Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printf("Creando tipo predefinido");

    /* Custom Data Type MPI */
    MPI_Datatype MPI_CELDA;
    int mpi_celda_block_lengths[] = {1, 1, 1, 1, 1, 1, 1, 1};
    MPI_Aint mpi_celda_displacements[] = {
        offsetof(Celda, cord_x),
        offsetof(Celda, cord_y),
        offsetof(Celda, estado),
        offsetof(Celda, edad),
        offsetof(Celda, vacunas),
        offsetof(Celda, grupo_riesgo),
        offsetof(Celda, sexo_biologico),
        offsetof(Celda, timer)};
    MPI_Datatype mpi_celda_lenghts[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    int cant_elem_to_send = frontera_columnas * world_size; // amount elements to send on Scatter.
    int total_values = frontera_filas * frontera_columnas;

    MPI_Type_create_struct(
        8,
        mpi_celda_block_lengths,
        mpi_celda_displacements,
        mpi_celda_lenghts,
        &MPI_CELDA);
    MPI_Type_commit(&MPI_CELDA);

    if (world_rank == 0)
    {
        printf("Inicializando Matriz \n");
        // Inicializando matriz.
        initMatrix(frontera_filas, frontera_columnas, tablero);
        start = 0;

       

        // Tablero Auxiliar
        tablero_auxiliar = (Celda *)malloc(frontera_filas * frontera_columnas * sizeof(Celda));
        if (tablero_auxiliar == NULL)
        {
            printf("No se ha podido reservar memoria");
        }

        buffer_resultado_auxiliar = (Celda *)malloc(cant_elem_to_send * sizeof(Celda));
        if (buffer_resultado == NULL)
        {
            printf("No se ha podido reservar memoria");
        }

        // Celda de prueba.
        Celda prueba;
        prueba.cord_x = 2;
        prueba.cord_y = 2;
        prueba.edad = 50;
        prueba.estado = ROJO;
        prueba.grupo_riesgo = 0;
        prueba.sexo_biologico = 1;
        prueba.timer = 14;
        prueba.vacunas = 1;

        printf("Matriz inicializada.. Cargando celda de prueba \n");
        // printf("Celda Cargada \n");
        // for (int i = 0; i < frontera_filas; i++)
        // {
        //     for (int j = 0; j < frontera_columnas; j++)
        //     {
        //         int k = (i * frontera_columnas) + j;
        //         mostrarColores(tablero[k]);
        //     }
        //     printf("\n");
        // }

        
    }
     // Simulación
        struct timeval ti, tf;
        double tiempo;
        double tiempo_total = 0;
        double tiempo_promedio = 0;

    submatrix = (Celda *)malloc(cant_elem_to_send * sizeof(Celda));
    if (submatrix == NULL)
    {
        printf("No se ha podido reservar memoria");
    }

    for (int i = 0; i < LAPS; i++)
    {
        if (world_rank == 0)
        {
            gettimeofday(&ti, NULL); // Tiempo inicial
        }

        printf("inicio de la simulacion\n");
        for (int i = 0; i < DIAS; i++)
        {
            // Dividir la matriz para cada proceso.
            for (int i = frontera_columnas; i < total_values; i += cant_elem_to_send)
            {

                getSubMatrix(tablero, submatrix, i, i + cant_elem_to_send); // obtener las filas.
                MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // Divido las respectivas filas Superiores de la matriz.
                getSubMatrix(tablero, submatrix, (i - frontera_columnas), (i - frontera_columnas) + cant_elem_to_send); // obtener las filas superiores

                MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_superior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // Divido las respectivas filas inferiores de la matriz.
                getSubMatrix(tablero, submatrix, (i + frontera_columnas), (i + frontera_columnas) + cant_elem_to_send); // obtener las filas superiores

                MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_inferior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // // printf("Comenzando actualizacion estados del proceso %d \n", world_rank);
                printf(" proceso %d fila :  \n", world_rank);
                // for (int i = 0; i < frontera_columnas; i++)
                // {
                //     mostrarColores(buffer_fila[i]);
                // }
                // printf("\n\n");

                if ((buffer_fila[0].cord_x == 0) || (buffer_fila[0].cord_x == frontera_filas - 1))
                {
                    for (int i = 0; i < frontera_columnas; i++)
                    {
                        buffer_fila[i].estado = CELESTE;
                        buffer_resultado[i] = buffer_fila[i];
                    }
                }
                else
                {

                    // Actualizo los estados de las celdas.
                    for (int i = 0; i < frontera_columnas; i++)
                    {
                        Calcular_Proximo_Estado(tablero, buffer_resultado, buffer_fila[i], buffer_fila, buffer_fila_superior, buffer_fila_inferior, frontera_filas, frontera_columnas);
                    }

                    // for (int i = 0; i < frontera_columnas; i++)
                    // {
                    //     mostrarColores(buffer_fila[i]);
                    // }
                    // printf("\n");

                    // printf("Soy el proceso %d y obtengo el siguiente resultado\n", world_rank);
                    // for (int i = 0; i < frontera_columnas; i++)
                    // {
                    //     mostrarColores(buffer_resultado[i]);
                    // }
                    // printf("\n\n");
                }

                MPI_Gather(buffer_resultado, frontera_columnas, MPI_CELDA, buffer_resultado_auxiliar, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                if (world_rank == 0)
                {

                    // Escribo lo que tiene el buffer en la porcion de la matriz auxiliar correspondiente.
                    int desplazamiento = start * cant_elem_to_send;
                    int padding = frontera_columnas; // Inserto el resultado desde la 1ra fila.
                    for (int i = 0; i < cant_elem_to_send; i++)
                    {
                        tablero_auxiliar[i + desplazamiento + padding] = buffer_resultado_auxiliar[i];
                    }

                    start += 1;
                }
            }
            if (world_rank == 0)
            {

                // Swap de punteros de la matriz.
                temp = tablero;
                tablero = tablero_auxiliar;
                tablero_auxiliar = temp;

                start = 0;
                // printf("\n Tablero Final \n");
                // for (int i = 0; i < frontera_filas; i++)
                // {
                //     for (int j = 0; j < frontera_columnas; j++)
                //     {
                //         int k = (i * frontera_columnas) + j;
                //         mostrarColores(tablero[k]);
                //         // tablero[k] = tablero_auxiliar[k];
                //     }
                //     printf("\n");
                // }

                // printf("Estadisticas : \n");

                // for (int i = 0; i < frontera_filas; i++)
                // {
                //     for (int j = 0; j < frontera_columnas; j++)
                //     {
                //         int k = (i * frontera_columnas) + j;
                //         printf("\n Celda:");
                //         printf("%d ", tablero[k].cord_x);
                //         printf("%d ", tablero[k].cord_y);
                //         // printf("%d", tablero[k].edad);
                //         mostrarColores(tablero[k]);

                //         // printf("%d", tablero[k].grupo_riesgo);
                //         // printf("%d", tablero[k].sexo_biologico);
                //         printf("%d ", tablero[k].timer);
                //         // printf("%d    ", tablero[k].vacunas);
                //     }
                //     printf("\n");
                // }
            }
        }
        if (world_rank == 0)
        {
            gettimeofday(&tf, NULL);
            tiempo = (tf.tv_sec - ti.tv_sec) * 1000.0 + (tf.tv_usec - ti.tv_usec) / 1000.0;
            printf("Has tardado: %g milisegundos\n", tiempo);
            tiempo_total += tiempo;
            tiempo = 0;
        }

        
    }
    if (world_rank == 0)
        {
            tiempo_promedio = tiempo_total / LAPS;
            printf("Tiempo Promedio en 10 corridas: %g milisegundos \n", tiempo_promedio);
        }
    MPI_Finalize();
}
