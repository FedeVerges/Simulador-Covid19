
//Colores
#define BLANCO 0   // libre
#define AZUL 1     // suceptible
#define NARANJA 2  // Enfermo sin contagio
#define ROJO 3     // Enfermo con contagio
#define AMARILLO 4 // Aislado
#define VERDE 5    // Curado
#define NEGRO 6    // Muerto
#define CELESTE -1
// Vecinos
#define IZQUIERDA -1
#define DERECHA 1
#define CENTRO 0
#define ARRIBA -1
#define ABAJO 1

#define VECINOS 8

typedef struct Celda
{
    int cord_x;
    int cord_y;
    int estado;         // color
    int edad;           // niño:[0-21],adulto:[22-64], anciano[65-120].
    int vacunas;        // 1:si, 0:no.
    int grupo_riesgo;   // 1:si, 0:no.
    int sexo_biologico; // 1:femenino, 0:Masculino.
    int timer;          // Temporizador para monitorear el avance de la enfermedad.
} Celda;


// Funciones de actualización de estados.

float Suceptible_Enf_sin_contagio(Celda Celda, int cantidad_vecinos, int cantidad_vecinos_infectados)
{
    float suceptibilidad = 0;
    float prob_contagio = 0;

    if (Celda.estado == AZUL)
    {
        // Calcular suceptibilidad.
        if (Celda.edad < 22)
        {
            suceptibilidad = 0.3;
            if (Celda.grupo_riesgo)
            {
                suceptibilidad += 0.15;
            }
        }
        else if (Celda.edad >= 22 && Celda.edad <= 64)
        {
            suceptibilidad = 0.5;
            if (Celda.grupo_riesgo)
            {
                suceptibilidad += 0.15;
            }
        }
        else if (Celda.edad > 64)
        {
            suceptibilidad = 0.9;
            if (Celda.grupo_riesgo)
            {
                suceptibilidad += 0.15;
            }
        }
        prob_contagio = (((cantidad_vecinos_infectados / cantidad_vecinos) * 2.4) + suceptibilidad) / 7;
        return prob_contagio;
    }
    return 0;
}

