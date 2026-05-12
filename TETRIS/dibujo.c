#include "GBT/gbt.h"
#include "dibujo.h"
#include "juego.h"
#define ANCHO_VENTANA 320
#include <string.h>
#define ALTO_VENTANA 200
#define ESCALA_VENTANA 1

unsigned char fuente[256][5][5] =
{
    ['T'] = {{1,1,1,1,1}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}},
    ['E'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['R'] = {{1,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,0}, {1,0,1,0,0}, {1,0,0,1,1}},
    ['I'] = {{0,1,1,1,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,0,1,0,0}, {0,1,1,1,0}},
    ['S'] = {{0,1,1,1,1}, {1,0,0,0,0}, {0,1,1,1,0}, {0,0,0,0,1}, {1,1,1,1,0}},
    ['J'] = {{0,0,1,1,1}, {0,0,0,1,0}, {0,0,0,1,0}, {1,0,0,1,0}, {0,1,1,0,0}},
    ['U'] = {{1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {0,1,1,1,0}},
    ['G'] = {{0,1,1,1,0}, {1,0,0,0,0}, {1,0,1,1,1}, {1,0,0,0,1}, {0,1,1,1,0}},
    ['A'] = {{0,1,1,1,0}, {1,0,0,0,1}, {1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}},
    ['L'] = {{1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['C'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,0,0,0,0}, {1,0,0,0,0}, {1,1,1,1,1}},
    ['O'] = {{1,1,1,1,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,0,0,0,1}, {1,1,1,1,1}},
    ['N'] = {{1,0,0,0,1}, {1,1,0,0,1}, {1,0,1,0,1}, {1,0,0,1,1}, {1,0,0,0,1}},
    ['F'] = {{1,1,1,1,1}, {1,0,0,0,0}, {1,1,1,1,0}, {1,0,0,0,0}, {1,0,0,0,0}},
    ['K'] = {{1,0,0,0,1}, {1,0,0,1,0}, {1,1,1,0,0}, {1,0,0,1,0}, {1,0,0,0,1}}
};

/*
void dibujarMenu(tJuego* juego, tBotones* jugar, tBotones* config, tBotones* salir, tBotones* ranking, tBotones* ayuda)
{
    TTF_Font* titulo;
    SDL_Texture* textTitulo;
    SDL_Surface* surfTitulo;
    SDL_Rect destTitulo;
    SDL_Color colorTexto = {255, 255, 255, 240}; //blanco
    int anchoTitulo, altoTitulo;

    titulo = TTF_OpenFont("fnt/Aero.ttf", 24);
    if (!titulo)
        printf("OpenFont: %s\n", TTF_GetError());

    TTF_SizeUTF8(titulo, "Simon Game", &anchoTitulo, &altoTitulo);

    destTitulo.x = 25;
    destTitulo.y = 50;
    destTitulo.w = anchoTitulo;
    destTitulo.h = altoTitulo;

    surfTitulo = TTF_RenderUTF8_Blended(titulo, "Simon Game", colorTexto);
    textTitulo = SDL_CreateTextureFromSurface(juego->render, surfTitulo);
    SDL_FreeSurface(surfTitulo);

    dibujarBotones(jugar, "Jugar", juego, 75, 90, "fnt/VCR_OSD_MONO_1.001.ttf", 16, colorTexto);
    dibujarBotones(config, "Configuracion", juego, 40, 110, "fnt/VCR_OSD_MONO_1.001.ttf", 16, colorTexto);
    dibujarBotones(ranking, "Ranking", juego, 65, 130, "fnt/VCR_OSD_MONO_1.001.ttf", 16, colorTexto);
    dibujarBotones(ayuda, "Ayuda", juego, 140, 20, "fnt/VCR_OSD_MONO_1.001.ttf", 16, colorTexto);
    dibujarBotones(salir, "Salir", juego, 75, 160, "fnt/VCR_OSD_MONO_1.001.ttf", 16, colorTexto);

    SDL_RenderCopy(juego->render, textTitulo, NULL, &destTitulo);
    SDL_RenderPresent(juego->render);

    SDL_DestroyTexture(textTitulo);
    SDL_DestroyTexture(jugar->textura);
    SDL_DestroyTexture(salir->textura);
    SDL_DestroyTexture(config->textura);
    SDL_DestroyTexture(ranking->textura);
    SDL_DestroyTexture(ayuda->textura);

    TTF_CloseFont(titulo);
    TTF_CloseFont(jugar->fuente);
    TTF_CloseFont(config->fuente);
    TTF_CloseFont(salir->fuente);
    TTF_CloseFont(ranking->fuente);
    TTF_CloseFont(ayuda->fuente);
}


void dibujarBotones(tBotones* boton, char* nombre, tJuego* juego, int x, int y, char* fuente, int tam, SDL_Color color)
{
    boton->fuente = TTF_OpenFont(fuente, tam);
    if (!boton->fuente)
        printf("OpenFont: %s\n", TTF_GetError());

    TTF_SizeUTF8(boton->fuente, nombre, &(boton->destino.w), &(boton->destino.h));

    boton->destino.x = x;
    boton->destino.y = y;

    boton->surface = TTF_RenderUTF8_Blended(boton->fuente, nombre, color);
    boton->textura = SDL_CreateTextureFromSurface(juego->render, boton->surface);
    SDL_FreeSurface(boton->surface);

    SDL_RenderCopy(juego->render, boton->textura, NULL, &(boton->destino));
}


void dibujarTextos(char* nombre, tJuego* juego, int x, int y, char* fuente, int tam, SDL_Color color)
{
    SDL_Surface* surface;
    SDL_Texture* textura;
    SDL_Rect destino;
    TTF_Font* font = TTF_OpenFont(fuente, tam);
    if (!font)
        printf("OpenFont: %s\n", TTF_GetError());

    TTF_SizeUTF8(font, nombre, &(destino.w), &(destino.h));

    destino.x = x;
    destino.y = y;

    surface = TTF_RenderUTF8_Blended(font, nombre, color);
    textura = SDL_CreateTextureFromSurface(juego->render, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(juego->render, textura, NULL, &destino);

    SDL_DestroyTexture(textura);
    TTF_CloseFont(font);
}
*/

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
/*
void renderMenu(tJuego *juego) {
    // Limpiamos el fondo primero (importante para no dejar rastro)
    gbt_borrar_backbuffer(0);

    for(int i = 0; i < 4; i++) { // Supongamos 4 opciones
        int y_pos = 60 + (i * 25);
        int color = (i == juego->opcionMenu) ? 7 : 3; // Color brillante si está seleccionado
        escribir_texto(50, 20, "TETRIS", 7);
        // Dibujamos un rectángulo de 60x15 píxeles
        dibujar_bloque(40, y_pos, 60, 15, color);
        escribir_texto(50, 70, "JUGAR", 7);
    }
    gbt_volcar_backbuffer();
    // Si la librería requiere presentar el buffer, recordá llamarlo en el main
}
*/

void renderMenu(tJuego *juego)
{
    gbt_borrar_backbuffer(0);

    int anchoTexto = strlen("TETRIS") * 8;
    int xTitulo = (juego->ancho_v - anchoTexto) / 2;
    int yTitulo = 30;

    int centroX = juego->ancho_v / 2;
    int centroY = juego->alto_v / 2;

    // 1. Dibujar el Título (UNA SOLA VEZ, fuera del for)
    escribir_texto(xTitulo, yTitulo, "TETRIS", 2);

    // 2. Dibujar las opciones
    char *nombres[] = {"JUGAR", "CONFIG", "RANKING", "SALIR"};

    for(int i = 0; i < 4; i++)
    {
        int y_pos = 100 + (i * 25); // Bajamos un poco el menú
        int colorBloque = (i == juego->opcionMenu) ? 7 : 3; // Blanco si seleccionado, gris si no
        int colorTexto = (i == juego->opcionMenu) ? 0 : 7;  // Texto negro sobre bloque blanco, o viceversa

        // Dibujamos el bloque
        dibujar_bloque(50, y_pos, 60, 15, colorBloque);

        // Dibujamos el texto centrado en el bloque (usamos y_pos + un pequeño offset)
        escribir_texto(55, y_pos + 5, nombres[i], colorTexto);
    }

    gbt_volcar_backbuffer();
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



void dibujarPieza(tPieza *p) {
    int offsetX = 120; // El mismo margen que uses para el tablero
    int offsetY = 30;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(p->forma[i][j]) {
                int x = offsetX + (p->px + j) * 8;
                int y = offsetY + (p->py + i) * 8;
                dibujar_bloque(x, y, 7, 7, 14); // 7x7 para que se vea el borde

            }
        }

    }
gbt_volcar_backbuffer();
}


void dibujarmapa(tJuego *juego, int** tablero,int ancho,int alto)
{
    // 1. Limpiamos con el color de fondo (por ejemplo, 0 para negro o el que quieras)
    gbt_borrar_backbuffer(0);


    // Calculamos el centro para el pozo
    int pozo_ancho = 80;  // 10 columnas * 8 px
    int pozo_alto = 160; // 20 filas * 8 px

    int offsetX = (ancho - pozo_ancho) / 2;
    int offsetY = (alto - pozo_alto) / 2;

    dibujar_marco(offsetX - 2, offsetY - 2, pozo_ancho + 4, pozo_alto + 4, 5);

    /*
    dibujar_bloque_estadisticas();
    dibujar_bloque_next();
    dibujar_score();


    for(int f = 0; f < 20; f++) {
        for(int c = 0; c < 10; c++) {
            if(tablero[f][c] != 0) {
                // Dibujamos un bloque en la posición relativa
                dibujar_bloque(offsetX + (c * 8), offsetY + (f * 8), tablero[f][c]);
            }
        }
    }
    */
    // 2. Dibujamos TODO lo que pertenece al juego
    //dibujar_bloque(50, 5, 60, 15, 5);
    // Aquí iría el dibujo del tablero, piezas, etc.

    // 3. RECIÉN AQUÍ volcamos todo junto a la pantalla física
    //gbt_volcar_backbuffer();
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

