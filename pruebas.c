
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>

void getSubMatrix(int matrix[], int submatrix[], int start, int finish)
{
    // get the matri'x part to send.
    for (int i = start; i < finish; i += 1)
    {
        submatrix[i - start] = matrix[i];
    }
}

int main(int argc, char **argv)
{
    int cant_columns = 12; // cant_columns = row lenght
    int cant_rows = 12;
    int total_values = cant_columns * cant_rows;

    int no_repeat = 0;   // To Print correctly the vector.
    int indexUnique = 0; // Index for print once the result vector

    int result_vector[cant_columns]; // Result Vector.

    int name_len;

    int recvRow[cant_columns]; // Shared array to send the values of matri'x columns.
    int matrix[total_values];  // Matrix
    int vector[cant_columns];  // Vector

    for (int i = 0; i < total_values; i += 1)
    {
        // Generate values.
        // Generando el vector con las filas de la matriz
        matrix[i] = i + 1;
    }
    for (int i = 0; i < cant_columns; i += 1)
    {
        // Generate Vector
        vector[i] = i + 1;
    }

    int cant_elements_to_send = (cant_columns*3); // amount elements to send on Scatter.
    int submatrix[cant_elements_to_send*3];           // Array to keep the part of the matrix to send on Scatter.
    int cantidad_procesos = 3;
    
    int *cells_per_proc;
    int *rows_per_proc;
    int *send_displacements;
    int *recv_displacements;

    int result; // Final result of the multiplication.

    for (int fila = 0; fila < cant_rows; fila+=cantidad_procesos)
    {

        for (int i = 0; i < cantidad_procesos; i++)
        {
            getSubMatrix(matrix, submatrix, i, i + cant_elements_to_send);
        }
    }


    // Calculate cant_rows & cells to process per slave proc
        cells_per_proc = (int *)malloc((size_t)cantidad_procesos * (int)sizeof(int));
        rows_per_proc = (int *)malloc((size_t)cantidad_procesos * (int)sizeof(int));
        for (int i = 0; i < cantidad_procesos; i++)
        {
            rows_per_proc[i] = (cant_rows / cantidad_procesos + (i < cant_rows % cantidad_procesos));
            cells_per_proc[i] = rows_per_proc[i] * cant_columns;
        }

        // Calculate the displacements
        send_displacements = (int *)malloc((size_t)cantidad_procesos * (int)sizeof(int));
        recv_displacements = (int *)malloc((size_t)cantidad_procesos * (int)sizeof(int));
        for (int i = 0; i < cantidad_procesos; i++)
        {
            int displacement = 0;
            for (int j = 0; j < i; j++)
            {
                displacement += rows_per_proc[j];
            }
            recv_displacements[i] = displacement;
            send_displacements[i] = displacement * cant_columns;
        }
    return 0;
}

/*


            if (buffer_fila[0].cord_x == 0 || buffer_fila[0].cord_x >= frontera_filas - 1)
            {
                // printf("soy el proceso %d y tengo la Fila:", world_rank);
                // for (int i = 0; i < frontera_filas; i++)
                // {
                //     mostrarColores(buffer_fila[i]);
                // }
                // printf("\n");

                // for (int i = 0; i < frontera_columnas; i++)
                // {
                //     buffer_resultado[i] = buffer_fila[i];
                // }
            }
            else
            {

                // // // Divido las respectivas filas Superiores de la matriz.
                getSubMatrix(tablero, submatrix, (i - frontera_columnas), (i - frontera_columnas) + cant_elem_to_send); // obtener las filas superiores
                MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_superior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // // // Divido las respectivas filas inferiores de la matriz.
                // getSubMatrix(tablero, submatrix, (i + frontera_columnas), (i + frontera_columnas) + cant_elem_to_send); // obtener las filas superiores
                // for (int i = 0; i < frontera_columnas; i++)
                // {
                //     mostrarColores(submatrix[i]);
                // }
                // printf("\n");
                // MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_inferior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // printf("soy el proceso %d y tengo la Fila:", world_rank);
                // for (int i = 0; i < frontera_filas; i++)
                // {
                //     mostrarColores(buffer_fila[i]);
                // }
                // printf("\n");

                // Actualizo los estados de las celdas.
                for (int i = 0; i < frontera_columnas; i++)
                {
                    Calcular_Proximo_Estado(tablero, buffer_resultado, buffer_fila[i], buffer_fila, buffer_fila_superior, buffer_fila_inferior, frontera_filas, frontera_columnas);
                }

                // printf("soy el proceso %d y mi fila resultado es:", world_rank);
                // for (int i = 0; i < frontera_columnas; i++)
                // {
                //     mostrarColores(buffer_resultado[i]);
                // }
                // printf("\n");
            }
            //MPI_Gather(buffer_resultado, frontera_columnas, MPI_CELDA, buffer_resultado_auxiliar, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);
            if (world_rank == 0)
            {
                // Escribo lo que tiene el buffer en la porcion de la matriz auxiliar correspondiente.
                int desplazamiento = start * cant_elem_to_send;
                for (int i = 0; i < cant_elem_to_send; i++)
                {

                    tablero_auxiliar[i + desplazamiento] = buffer_resultado_auxiliar[i];
                }
                start += 1;
            }
        }
        if (world_rank == 0)
        {

            // // Swap de punteros de la matriz.
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


*/