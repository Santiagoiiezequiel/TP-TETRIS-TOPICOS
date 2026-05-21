#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include "dibujo.h"
#include "GBT/gbt.h"
#define COL 10
#define FILAS 20
#define LIMITE_IZQUIERDO 0
#define LIMITE_DERECHO 7

static const int tetrominos[7][4][4] =
{
   // 0: I (Barra) -> tam = 4 (Ocupa una fila completa pegada arriba para rotar bien)
    {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    },
    // 1: O (Cuadrado) -> tam = 2 (Metido exacto en el cuadrante 2x2 superior izquierdo)
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // 2: L -> tam = 3 (Apretado arriba a la izquierda en el bloque 3x3)
    {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    },
    // 3: J -> tam = 3 (Apretado arriba a la izquierda en el bloque 3x3)
    {
        {0, 0, 1, 0},
        {0, 0, 1, 0},
        {0, 1, 1, 0}, // El bloque sobresale hacia la izquierda ocupando el 3x3
        {0, 0, 0, 0}
    },
    // 4: Z -> tam = 3 (Subido a la fila 0 y corrido a la columna 0)
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0}
    },
    // 5: S -> tam = 3 (Subido a la fila 0 y corrido a la columna 0)
    {
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0}
    },
    // 6: T -> tam = 3 (Este ya lo tenías perfecto)
    {
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0}
    }

    // T, L, J, S, Z...
};

void iniciarJuego(tJuego *juego, int** tablero)
{
    // 1. Limpiar el tablero (suponiendo FILAS 20 y COLS 10)
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            tablero[i][j] = 0;
        }
    }

    // 2. Resetear estadísticas
    juego->puntaje = 0;
    juego->lineas = 0;
    juego->nivel = 1;

    // 3. Generar la primera pieza
    //generarNuevoTetramino(&juego);

    // 4. Cambiar la instancia
    //juego->instancia = JUGANDO;
}
/*
void generarNuevoTetramino(tPieza *p)
{

    crearPieza(p);


}
*/

int verificar_y_limpiar_lineas(int **tablero) {
    int lineas_borradas = 0;

    // Recorremos el tablero de abajo hacia arriba (FILAS = 20, COL = 10)
    for (int i = 19; i >= 0; i--) {
        int fila_completa = 1;

        // Comprobamos si toda la fila está llena (ningún casillero es 0)
        for (int j = 0; j < 10; j++) {
            if (tablero[i][j] == 0) {
                fila_completa = 0;
                break;
            }
        }

        // ¡Si encontramos una fila llena!
        if (fila_completa) {
            lineas_borradas++;

            // Guardamos el puntero de la fila que va a desaparecer para no perder la memoria
            int *fila_a_vaciar = tablero[i];

            // Desplazamos los punteros de todas las filas superiores una posición hacia abajo
            for (int k = i; k > 0; k--) {
                tablero[k] = tablero[k - 1];
            }

            // La fila vieja que quedó "flotando" la ponemos arriba de todo como nueva fila vacía
            tablero[0] = fila_a_vaciar;

            // Limpiamos sus datos poniéndole ceros
            for (int j = 0; j < 10; j++) {
                tablero[0][j] = 0;
            }

            // Como bajamos todo, tenemos que volver a revisar la misma posición 'i'
            // ya que la fila que bajó podría estar llena también.
            i++;
        }
    }
    return lineas_borradas;
}


void fijar_pieza(tPieza *p, int **tablero) {
    for (int i = 0; i < p->tam; i++) {
        for (int j = 0; j < p->tam; j++) {
            if (p->matriz[i][j] != 0) {
                // Pasamos el color al casillero correspondiente del tablero
                tablero[p->py + i][p->px + j] = p->matriz[i][j];
            }
        }
    }
}


int comprobar_colision(int x, int y, int **matriz, int tam, int **tablero)
{
    for(int f = 0; f < tam; f++)
    {
        for(int c = 0; c < tam; c++)
        {
            // Solo analizamos bloques ocupados de la pieza
            if(matriz[f][c] != 0)
            {
                int tableroX = x + c;
                int tableroY = y + f;

                // -----------------------------
                // COLISIÓN CON BORDES
                // -----------------------------

                // izquierda
                if(tableroX < 0)
                    return 1;

                // derecha
                if(tableroX >= COL)
                    return 1;

                // abajo
                if(tableroY >= FILAS)
                    return 1;

                // -----------------------------
                // COLISIÓN CON OTRA PIEZA
                // -----------------------------

                if(tableroY >= 0 &&
                        tablero[tableroY][tableroX] != 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

/*
void rotar_pieza(tPieza *p,int** tablero)
{

    int tam = p->tam;

    // 1. Creamos un NUEVO array de punteros (las nuevas filas)
    // Pero OJO: no reservamos memoria para los enteros individuales (las columnas),
    // vamos a reutilizar los vectores que ya existen.
    int **nuevas_filas = (int **)malloc(tam * sizeof(int *));

    // 2. Aplicamos la lógica de rotación REORDENANDO los punteros de las filas
    // En lugar de mover elemento por elemento, reasignamos a dónde apunta cada fila.
    for (int f = 0; f < tam; f++)
    {
        // Buscamos cuál de las filas originales se convertirá en la nueva fila 'f'
        // después de girar 90 grados.
        nuevas_filas[f] = p->matriz[(tam - 1) - f];
    }



    if (!comprobar_colision(p->px, p->py, nuevas_filas, tam, tablero))
    {

        // ¡La rotación es válida!
        // Liberamos el viejo "index" de punteros de la pieza (solo el array de arriba)
        free(p->matriz);

        // Hacemos que la pieza adopte el nuevo orden de punteros
        p->matriz = nuevas_filas;

        // NOTA MATEMÁTICA: La rotación pura de punteros de filas nos deja una matriz transpuesta.
        // Para completar el giro horizontal de los elementos internos sin reescribir todo,
        // simplemente invertimos el orden de las columnas en el dibujado o mediante un swap rápido de punteros.

    }
    else
    {
        // Si colisiona, abortamos. Liberamos el array temporal de punteros.
        // Los datos reales de la pieza quedan intactos porque nunca los tocamos.
        free(nuevas_filas);
    }



}
*/
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


void crearPieza(tPieza *p)
{
    int tipo = rand()%7;
    //int num=0;
    p->num = tipo;

    switch(p->num)
    {
    case 0:
        p->color = 11;
        p->tam=4;
        break;
    case 1:
        p->color = 14;
        p->tam=4;
        break;
    case 2:
        p->color =15;
        p->tam=3;
        break;
    case 3:
        p->color = 1;
        p->tam=3;
        break;
    case 4:
        p->color = 4;
        p->tam=4;
        break;
    case 5:
        p->color = 2;
        p->tam=3;
        break;
    case 6:
        p->color = 13;
        p->tam=3;
        break;
    }


    p->matriz = (int **)malloc(p->tam * sizeof(int *));
    for (int i = 0; i < p->tam; i++)
    {
        p->matriz[i] = (int *)malloc(p->tam * sizeof(int));
    }

    // 3. Copiamos los datos desde tu array original 'tetrominos'
    // Pero solo copiamos hasta 'p->tam', evitando tocar memoria de más
    for (int i = 0; i < p->tam; i++)
    {
        for (int j = 0; j < p->tam; j++)
        {

            // Si en tu matriz 'tetrominos' hay un 1 (bloque ocupado),
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
