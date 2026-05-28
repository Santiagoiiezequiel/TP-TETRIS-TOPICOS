#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include "dibujo.h"
#include "GBT/gbt.h"
#define COL 10
#define FILAS 20
#define LIMITE_IZQUIERDO 0
#define LIMITE_DERECHO 7
#define CONFIG_FILE "config.txt"

int tetrominos[7][4][4] =
{
    // 0: Barra (I) -> Requiere 4x4 de espacio real para girar bien
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 1: Cuadrado (O) -> Entra perfecto en 2x2 (filas 0 y 1, col 0 y 1)
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 2: Te (T) -> Entra en 3x3 (filas 0 y 1, col 0, 1 y 2)
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 3: L -> Entra en 3x3
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 4: J -> Entra en 3x3
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 5: S -> Entra en 3x3
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // 6: Z -> Entra en 3x3
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

void iniciarJuego(tJuego *juego, int** tablero)
{
    //1. Limpiar el tablero (suponiendo FILAS 20 y COLS 10)
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            tablero[i][j] = 0;
        }
    }

    // 2. Resetear estadísticas


    juego->puntaje = 0;
    juego->lineas_limpiadas = 0;
    juego->piezas_caidas = 0;
    juego->nivel = 1;
    juego->velocidad_actual = 1.0f;

}

void guardarConfiguracion(tJuego *juego)
{
    FILE *archivo = fopen(CONFIG_FILE, "w");
    if (archivo == NULL) return;

    // Accedemos a través de juego->config.variable
    fprintf(archivo, "%d\n", juego->config.paleta);
    fprintf(archivo, "%d\n", juego->config.resolucion);
    fprintf(archivo, "%f\n", juego->config.velocidad_caida); // Guardamos como float

    fclose(archivo);
}

void cargarConfiguracion(tJuego *juego)
{
    FILE *archivo = fopen(CONFIG_FILE, "r");

    if (archivo == NULL)
    {
        // Valores por defecto usando tus floats
        juego->config.paleta = 0;
        juego->config.resolucion = 0;
        juego->config.velocidad_caida = 1.0f; // 1000ms base
        juego->opcionConfig = 0;
        guardarConfiguracion(juego);
        return;
    }

    fscanf(archivo, "%d", &juego->config.paleta);
    fscanf(archivo, "%d", &juego->config.resolucion);
    fscanf(archivo, "%f", &juego->config.velocidad_caida); // Leemos como float
    juego->opcionConfig = 0;

    fclose(archivo);
}


int verificar_y_limpiar_lineas(int **tablero)
{
    int lineas_borradas = 0;

    // Recorremos el tablero de abajo hacia arriba (FILAS = 20, COL = 10)
    for (int i = 19; i >= 0; i--)
    {
        int fila_completa = 1;

        // Comprobamos si toda la fila está llena (ningún casillero es 0)
        for (int j = 0; j < 10; j++)
        {
            if (tablero[i][j] == 0)
            {
                fila_completa = 0;
                break;
            }
        }

        // ¡Si encontramos una fila llena!
        if (fila_completa)
        {
            lineas_borradas++;

            // Guardamos el puntero de la fila que va a desaparecer para no perder la memoria
            int *fila_a_vaciar = tablero[i];

            // Desplazamos los punteros de todas las filas superiores una posición hacia abajo
            for (int k = i; k > 0; k--)
            {
                tablero[k] = tablero[k - 1];
            }

            // La fila vieja que quedó "flotando" la ponemos arriba de todo como nueva fila vacía
            tablero[0] = fila_a_vaciar;

            // Limpiamos sus datos poniéndole ceros
            for (int j = 0; j < 10; j++)
            {
                tablero[0][j] = 0;
            }

            // Como bajamos todo, tenemos que volver a revisar la misma posición 'i'
            // ya que la fila que bajó podría estar llena también.
            i++;
        }
    }
    return lineas_borradas;
}


void fijar_pieza(tPieza *p, int **tablero)
{
    for (int i = 0; i < p->tam; i++)
    {
        for (int j = 0; j < p->tam; j++)
        {
            if (p->matriz[i][j] != 0)
            {
                int tableroX = p->px + j;
                int tableroY = p->py + i;

                // Guardamos en el tablero solo si está dentro de los márgenes reales
                if (tableroX >= 0 && tableroX < 10 && tableroY >= 0 && tableroY < 20)
                {
                    tablero[tableroY][tableroX] = p->color; // Guardamos su color real
                }
            }
        }
    }
}

int comprobar_colision(int px, int py, int **matriz, int tam, int **tablero)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            // Verificamos si la celda del molde del tetramino tiene un bloque (distinto de 0)
            if (matriz[i][j] != 0)
            {
                int tableroX = px + j;
                int tableroY = py + i;

                // 1. Validamos límites laterales e inferiores absolutos de la grilla (10x20)
                if (tableroX < 0 || tableroX >= 10 || tableroY >= 20)
                {
                    return 1; // Colisión con bordes o con el fondo del pozo
                }

                // 2. Si la pieza está naciendo arriba de todo (Y negativo), no testea bloques fijos
                if (tableroY < 0)
                {
                    continue;
                }

                // 3. Validamos si esa posición ya está ocupada por otra pieza fija
                if (tablero[tableroY][tableroX] != 0)
                {
                    return 1; // Colisión contra bloque estático
                }
            }
        }
    }
    return 0;
}

void rotar_pieza(tPieza *p, int **tablero)
{
    int tam = p->tam;

    // 1. Reservamos nueva matriz
    int **rotada = malloc(sizeof(int*) * tam);

    for(int i = 0; i < tam; i++)
    {
        rotada[i] = malloc(sizeof(int) * tam);
    }

    // 2. Rotamos correctamente
    for(int f = 0; f < tam; f++)
    {
        for(int c = 0; c < tam; c++)
        {
            rotada[c][tam - 1 - f] = p->matriz[f][c];
        }
    }

    // 3. Verificamos colisión
    if(!comprobar_colision(p->px, p->py, rotada, tam, tablero))
    {
        // Liberamos vieja matriz
        for(int i = 0; i < tam; i++)
        {
            free(p->matriz[i]);
        }

        free(p->matriz);

        // Asignamos la nueva
        p->matriz = rotada;
    }
    else
    {
        // Si colisiona, liberamos la rotada
        for(int i = 0; i < tam; i++)
        {
            free(rotada[i]);
        }

        free(rotada);
    }
}

int obtenerPiezaAleatoria()
{

    static int bolsaTetrominos[7] = {0, 1, 2, 3, 4, 5, 6};
    static int indiceBolsa = 7; // Arranca en 7 para forzar la primera mezcla
    // Si ya sacamos las 7 piezas, rellenamos la bolsa y la volvemos a mezclar
    if (indiceBolsa >= 7)
    {
        // Reiniciamos los tipos de pieza (0 a 6)
        for (int i = 0; i < 7; i++)
        {
            bolsaTetrominos[i] = i;
        }

        // Algoritmo de mezcla aleatoria pura (Fisher-Yates)
        for (int i = 6; i > 0; i--)
        {
            int j = rand() % (i + 1);
            // Intercambiamos los valores
            int aux = bolsaTetrominos[i];
            bolsaTetrominos[i] = bolsaTetrominos[j];
            bolsaTetrominos[j] = aux;
        }

        indiceBolsa = 0; // Reseteamos el índice para empezar a sacar de la nueva bolsa
    }

    // Sacamos la pieza actual y sumamos uno al índice para la próxima
    int tipoPieza = bolsaTetrominos[indiceBolsa];
    indiceBolsa++;

    return tipoPieza;
}

void crearPieza(tPieza *p)
{
    int tipo = obtenerPiezaAleatoria();
    //int num=0;
    p->num = tipo;

    switch(p->num)
    {
    case 0: // Barra (I)
        p->color = 11;
        p->tam = 4; //
        break;
    case 1: // Cuadrado (O)
        p->color = 14;
        p->tam = 2; //
        break;
    case 2: // Te (T)
        p->color = 15;
        p->tam = 3;
        break;
    case 3: // L
        p->color = 1;
        p->tam = 3;
        break;
    case 4: // J
        p->color = 4;
        p->tam = 3;
        break;
    case 5: // S
        p->color = 2;
        p->tam = 3;
        break;
    case 6: // Z
        p->color = 13;
        p->tam = 3;
        break;
    }


    p->matriz = (int **)malloc(p->tam * sizeof(int *));
    for (int i = 0; i < p->tam; i++)
    {
        p->matriz[i] = (int *)malloc(p->tam * sizeof(int));
    }


    for (int i = 0; i < p->tam; i++)
    {
        for (int j = 0; j < p->tam; j++)
        {

            // Si en la matriz hay un 1 (bloque ocupado),
            // le guardamos su color. Si hay un 0, guardamos 0.
            if (tetrominos[p->num][i][j] != 0)
            {
                p->matriz[i][j] = p->color;
            }
            else
            {
                p->matriz[i][j] = 0;
            }
        }
    }

    /*
      for(int i = 0; i < 4; i++)
      {
          for(int j = 0; j < 4; j++)
          {
              p->forma[i][j] = tetrominos[tipo][i][j];


          }
      }
    */



    p->px = 3;
    p->py = 0;
}


int verificar_y_limpiar_lineas_punteros(int **tablero, int ancho, int alto, tJuego *juego)
{
    int lineas_eliminadas = 0;
    int filas_a_animar[20] = {0}; // Array para marcar cuáles filas se completaron
    int hay_lineas = 0;

    // 1. PRIMER PASO: Detectar y marcar qué filas están llenas
    for (int f = 19; f >= 0; f--)
    {
        int fila_completa = 1;
        for (int c = 0; c < 10; c++)
        {
            if (tablero[f][c] == 0)
            {
                fila_completa = 0;
                break;
            }
        }
        if (fila_completa)
        {
            filas_a_animar[f] = 1; // Marcamos que la fila 'f' debe parpadear
            hay_lineas = 1;
        }
    }

    // Si no hay ninguna línea completa, salimos rápido sin perder tiempo
    if (!hay_lineas)
    {
        return 0;
    }

    // 2. SEGUNDO PASO: Efecto visual de parpadeo
    // Guardamos una copia temporal de los colores originales de esas filas para el parpadeo
    int copia_colores[20][10];
    for (int f = 0; f < 20; f++)
    {
        if (filas_a_animar[f])
        {
            for (int c = 0; c < 10; c++)
            {
                copia_colores[f][c] = tablero[f][c];
            }
        }
    }

    for (int parpadeo = 0; parpadeo < 3; parpadeo++)
    {
        // FASE A: Pintar de BLANCO
        for (int f = 0; f < 20; f++)
        {
            if (filas_a_animar[f])
            {
                for (int c = 0; c < 10; c++)
                {
                    tablero[f][c] = 7;
                }
            }
        }

        gbt_borrar_backbuffer(0);
        dibujarmapa(juego, tablero, ancho, alto);
        gbt_volcar_backbuffer();
        gbt_esperar(80);

        // FASE B: Pintar de NEGRO (Color 0) para simular el apagado
        for (int f = 0; f < 20; f++)
        {
            if (filas_a_animar[f])
            {
                for (int c = 0; c < 10; c++)
                {
                    tablero[f][c] = 0;
                }
            }
        }
        gbt_borrar_backbuffer(0);
        dibujarmapa(juego, tablero, ancho, alto);
        gbt_volcar_backbuffer();
        gbt_esperar(80);
    }

    // Restauramos los colores originales antes del procesamiento lógico por punteros
    for (int f = 0; f < 20; f++)
    {
        if (filas_a_animar[f])
        {
            for (int c = 0; c < 10; c++)
            {
                tablero[f][c] = copia_colores[f][c];
            }
        }
    }

    // 3. TERCER PASO: El borrado físico real intercambiando los punteros
    for (int f = 19; f >= 0; f--)
    {
        if (filas_a_animar[f])
        {
            lineas_eliminadas++;

            int *fila_a_vaciar = tablero[f];

            // Vaciamos los datos de esa fila
            for (int c = 0; c < 10; c++)
            {
                fila_a_vaciar[c] = 0;
            }

            // Desplazamos los punteros de las filas superiores hacia abajo
            for (int k = f; k > 0; k--)
            {
                tablero[k] = tablero[k - 1];
            }

            // La fila vieja y limpia pasa a ser el nuevo techo (fila 0)
            tablero[0] = fila_a_vaciar;

            // Como los punteros se movieron, también tenemos que desplazar el
            // array de marcas "filas_a_animar" para que sigan coincidiendo los índices
            for (int k = f; k > 0; k--)
            {
                filas_a_animar[k] = filas_a_animar[k - 1];
            }
            filas_a_animar[0] = 0;


            f++;
        }
    }

    if (lineas_eliminadas > 0)
    {
        int puntosBase = 0;
        if (lineas_eliminadas == 1) puntosBase = 100;
        else if (lineas_eliminadas == 2) puntosBase = 300;
        else if (lineas_eliminadas == 3) puntosBase = 500;
        else if (lineas_eliminadas == 4) puntosBase = 800;

        float multiplicadorVelocidad = 1.0f / juego->velocidad_actual;

        // ¡CRÍTICO!: Modificar usando el puntero
        juego->puntaje += (int)(puntosBase * multiplicadorVelocidad);
    }

    return lineas_eliminadas;
}
