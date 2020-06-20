/*
            if (buffer_fila[0].cord_x == 0)
            {
                for (int i = 0; i < frontera_columnas; i++)
                {
                    buffer_resultado[i] = buffer_fila[i];
                }

                // getSubMatrix(tablero, submatrix, (i + frontera_columnas), (i + frontera_columnas) + cant_elem_to_send); // obtener las filas superiores
                // printf("Scatter filas inferiores \n");
                // MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_inferior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);
            }
            else if (buffer_fila[0].cord_x == frontera_filas - 1)
            {
                for (int i = 0; i < frontera_columnas; i++)
                {
                    buffer_resultado[i] = buffer_fila[i];
                }
                printf("soy el proceso %d y tengo la ULTIMA FILA:", world_rank);
                for (int i = 0; i < frontera_filas; i++)
                {
                    mostrarColores(buffer_fila[i]);
                    // mostrarColores(buffer_resultado[i]);
                }
                printf("\n");
                printf("Fin actualizacion estados del proceso %d \n", world_rank);

                // getSubMatrix(tablero, submatrix, (i - frontera_columnas), (i - frontera_columnas) + cant_elem_to_send); // obtener las filas superiores

                //MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_superior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);
            }
            else
            {

                printf("soy el proceso %d y tengo la Fila:", world_rank);
                for (int i = 0; i < frontera_filas; i++)
                {
                    mostrarColores(buffer_fila[i]);
                }
                printf("\n\n");

                // Divido las respectivas filas Superiores de la matriz.
                getSubMatrix(tablero, submatrix, (i - frontera_columnas), (i - frontera_columnas) + cant_elem_to_send); // obtener las filas superiores
               
                retorno = MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_superior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

                // Divido las respectivas filas inferiores de la matriz.
                getSubMatrix(tablero, submatrix, (i + frontera_columnas), (i + frontera_columnas) + cant_elem_to_send); // obtener las filas superiores
                

                MPI_Scatter(submatrix, frontera_columnas, MPI_CELDA, buffer_fila_inferior, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);


                printf("Comenzando actualizacion estados del proceso %d \n", world_rank);

                // Actualizo los estados de las celdas.
                for (int i = 0; i < frontera_columnas; i++)
                {
                    Calcular_Proximo_Estado(tablero, buffer_resultado, buffer_fila[i], buffer_fila, buffer_fila_superior, buffer_fila_inferior, frontera_filas, frontera_columnas);
                }
                printf("Fin actualizacion estados del proceso %d \n", world_rank);

                // printf("soy el proceso %d y mi fila resultado es:", world_rank);
                // for (int i = 0; i < frontera_columnas; i++)
                // {
                //     mostrarColores(buffer_resultado[i]);
                // }
                // printf("\n");
            }
            MPI_Gather(buffer_resultado, frontera_columnas, MPI_CELDA, buffer_resultado_auxiliar, frontera_columnas, MPI_CELDA, 0, MPI_COMM_WORLD);

            if (world_rank == 0)
            {
                // Escribo lo que tiene el buffer en la porcion de la matriz auxiliar correspondiente.
                // int desplazamiento = start * cant_elem_to_send;
                // for (int i = 0; i < cant_elem_to_send; i++)
                // {
                //     tablero_auxiliar[i + desplazamiento] = buffer_resultado_auxiliar[i];
                // }
                // start += 1;
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
    }


*/