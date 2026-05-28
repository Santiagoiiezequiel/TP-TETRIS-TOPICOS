#include "GBT/gbt.h"
#include "dibujo.h"
#include "juego.h"
#define ANCHO_VENTANA 320
#include <string.h>
#define ALTO_VENTANA 200
#define ESCALA_VENTANA 1
#define COL 10
#define FILAS 20

unsigned char fuente[256][5][5] =
{
    ['A'] = {{0,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}},
    ['B'] = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}},
    ['C'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['D'] = {{1,1,1,1,0}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,0}},
    ['E'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['F'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}},
    ['G'] = {{0,1,1,1,0}, {1,0,0,0,0}, {1,0,1,1,1}, {1,0,0,0,1}, {0,1,1,1,0}},
    ['H'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}},
    ['I'] = {{0,1,1,1,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,1,1,1,0}},
    ['J'] = {{0,0,1,1,1}, {0,0,0,1,0}, {0,0,0,1,0}, {1,0,0,1,0}, {0,1,1,0,0}},
    ['K'] = {{1,0,0,0,1}, {1,0,0,1,0}, {1,1,1,0,0}, {1,0,0,1,0}, {1,0,0,0,1}},
    ['L'] = {{1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['M'] = {{1,0,0,0,1}, {1,1,0,1,1}, {1,0,1,0,1}, {1,0,0,0,1}, {1,0,0,0,1}},
    ['N'] = {{1,0,0,0,1}, {1,1,0,0,1}, {1,0,1,0,1}, {1,0,0,1,1}, {1,0,0,0,1}},
    ['O'] = {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}},
    ['P'] = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}},
    ['Q'] = {{0,1,1,1,0}, {1,0,0,0,1}, {1,0,1,0,1}, {1,0,0,1,0}, {0,1,1,0,1}},
    ['R'] = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,1,0,0}, {1,0,0,1,1}},
    ['S'] = {{0,1,1,1,1}, {1,0,0,0,0}, {0,1,1,1,0}, {0,0,0,0,1}, {1,1,1,1,0}},
    ['T'] = {{1,1,1,1,1}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}},
    ['U'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,1,1,0}},
    ['V'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}},
    ['W'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,1,0,1}, {1,1,0,1,1}, {1,0,0,0,1}},
    ['X'] = {{1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}, {0,1,0,1,0}, {1,0,0,0,1}},
    ['Y'] = {{1,0,0,0,1}, {0,1,0,1,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}},
    ['Z'] = {{1,1,1,1,1}, {0,0,0,1,0}, {0,0,1,0,0}, {0,1,0,0,0}, {1,1,1,1,1}},

    // Espacio en blanco (Opcional, pero clave para que no printee basura)
    [' '] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}},
    ['0'] = {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}},
    ['1'] = {{0,0,1,0,0}, {0,1,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {1,1,1,1,1}},
    ['2'] = {{1,1,1,1,1}, {0,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['3'] = {{1,1,1,1,1}, {0,0,0,0,1}, {0,1,1,1,1}, {0,0,0,0,1}, {1,1,1,1,1}},
    ['4'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}, {0,0,0,0,1}, {0,0,0,0,1}},
    ['5'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,1}, {0,0,0,0,1}, {1,1,1,1,1}},
    ['6'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,1}, {1,0,0,0,1}, {1,1,1,1,1}},
    ['7'] = {{1,1,1,1,1}, {0,0,0,0,1}, {0,0,1,0,0}, {0,1,0,0,0}, {0,1,0,0,0}},
    ['8'] = {{1,1,1,1,1}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,1,1,1,1}},
    ['9'] = {{1,1,1,1,1}, {1,0,0,0,1}, {1,1,1,1,1}, {0,0,0,0,1}, {1,1,1,1,1}}
};



void escribir_texto(int x, int y, const char* texto, int color)
{
    int separacion = 6; // 5 píxeles de la letra + 1 de espacio

    for (int i = 0; texto[i] != '\0'; i++)
    {
        char letra = texto[i];

        // Dibujamos la matriz de la letra actual
        for (int fila = 0; fila < 5; fila++)
        {
            for (int col = 0; col < 5; col++)
            {
                if (fuente[(int)letra][fila][col] == 1)
                {
                    gbt_dibujar_pixel(x + (i * separacion) + col, y + fila, color);
                }
            }
        }
    }
}


void escribir_texto_escalado(int x, int y, const char *texto, int color, int escala)
{
    int i = 0;
    // Recorremos cada letra de la palabra
    while (texto[i] != '\0')
    {
        char c = texto[i];

        // Recorremos la matriz de 5x5 de la fuente para esa letra
        for (int fila = 0; fila < 5; fila++)
        {
            for (int col = 0; col < 5; col++)
            {
                // Si el bit está encendido, dibujamos un bloque de tamaño (escala x escala)
                if (fuente[(unsigned char)c][fila][col] == 1)
                {
                    int px = x + (col * escala);
                    int py = y + (fila * escala);
                    dibujar_bloque(px, py, escala, escala, color);
                }
            }
        }

        // Avanzamos la X para la siguiente letra:
        // (5 píxeles de la letra + 1 de espacio) multiplicado por la escala
        x += (5 + 1) * escala;
        i++;
    }
}

void dibujar_bloque_tetris(int x, int y, int ancho,int alto, int color)
{
    // 1. Dibujamos el centro sólido con el color de la pieza
    for (int i = 1; i < alto - 1; i++)
    {
        for (int j = 1; j < alto - 1; j++)
        {
            gbt_dibujar_pixel(x + i, y + j, color);
        }
    }

    // 2. Brillo superior (Línea horizontal superior blanca o tono claro = 15 o 7)
    // Usamos color 15 (Blanco brillante) para el reflejo de luz
    for (int i = 0; i < alto - 1; i++)
    {
        gbt_dibujar_pixel(x + i, y, 7);
    }

    // 3. Brillo izquierdo (Línea vertical izquierda blanca)
    for (int j = 0; j < alto - 1; j++)
    {
        gbt_dibujar_pixel(x, y + j, 8);
    }

    // 4. Sombra inferior (Línea horizontal inferior gris oscuro = 8)
    for (int i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x + i, y + (alto - 1), 8);
    }

    // 5. Sombra derecha (Línea vertical derecha gris oscuro)
    for (int j = 0; j < alto; j++)
    {
        gbt_dibujar_pixel(x + (alto - 1), y + j, 8);
    }
}

void dibujar_bloque(int x, int y, int ancho, int alto, int color)
{
    for (int i = 0; i < ancho; i++)
    {
        for (int j = 0; j < alto; j++)
        {
            gbt_dibujar_pixel(x + i, y + j, color);
        }
    }
}


void dibujar_marco(int x, int y , int ancho,int alto , int color)
{ // línea superior
    int i;
    for(i = 0; i < ancho; i++)
    {
        gbt_dibujar_pixel(x + i, y, color);
    }

    // línea inferior
    for(i = 0; i < ancho; i++)
    {
        gbt_dibujar_pixel(x + i, y + alto - 1, color);
    }

    // línea izquierda
    for(i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x, y + i, color);
    }

    // línea derecha
    for(i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x + ancho - 1, y + i, color);
    }



}



void dibujarPieza(tPieza *p, int ancho, int alto) {
    // 1. Determinar el tamaño del bloque según la resolución
    int tam_bloque = (ancho <= 320) ? 8 : 16;
    int tam_interno = tam_bloque - 1; // 7 para CGA, 15 para VGA

    // 2. Calcular las dimensiones del pozo dinámicamente
    int pozo_ancho = COL * tam_bloque;   // 10 * tam_bloque
    int pozo_alto = FILAS * tam_bloque;  // 20 * tam_bloque

    int offsetX = (ancho - pozo_ancho) / 2;
    int offsetY = (alto - pozo_alto) / 2;

    for(int i = 0; i < p->tam; i++) {
        for(int j = 0; j < p->tam; j++) {
            if(p->matriz[i][j]) {
                // Multiplicamos por el tamaño de bloque dinámico
                int x = offsetX + (p->px + j) * tam_bloque;
                int y = offsetY + (p->py + i) * tam_bloque;

                // Dibujamos con el tamaño adaptado
                dibujar_bloque_tetris(x, y, tam_interno, tam_interno, p->color);
            }
        }
    }
}


void dibujarmapa(tJuego *juego, int** tablero, int ancho, int alto)
{
    // 1. Determinar el tamaño del bloque según la resolución
    int tam_bloque = (ancho <= 320) ? 8 : 16;
    int tam_interno = tam_bloque - 1;

    // 2. Calcular dimensiones del pozo
    int pozo_ancho = COL * tam_bloque;
    int pozo_alto = FILAS * tam_bloque;

    int offsetX = (ancho - pozo_ancho) / 2;
    int offsetY = (alto - pozo_alto) / 2;

    // El marco rosa también se adapta al tamaño del pozo escalado
    dibujar_marco(offsetX - 2, offsetY - 2, pozo_ancho + 4, pozo_alto + 4, 5);

    for(int f = 0; f < FILAS; f++)
    {
        for(int c = 0; c < COL; c++)
        {
            if(tablero[f][c] != 0)
            {
                // Multiplicamos por el tamaño dinámico
                int x = offsetX + (c * tam_bloque);
                int y = offsetY + (f * tam_bloque);

                dibujar_bloque_tetris(x, y, tam_interno, tam_interno, tablero[f][c]);
            }
        }
    }
}

void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY)
{
    uint16_t offsetX = oX * (PIXELES_X_LADO + PX_PADDING);
    uint16_t offsetY = oY * (PIXELES_X_LADO + PX_PADDING);

    for (uint16_t y = 0; y < PIXELES_X_LADO; y++)
    {
        for (uint16_t x = 0; x < PIXELES_X_LADO; x++)
        {

            gbt_dibujar_pixel(offsetX + x, offsetY + y, dibujo[y][x]);
        }
    }
}

