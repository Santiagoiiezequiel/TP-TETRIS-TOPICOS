#ifndef DIBUJO_H_INCLUDED
#define DIBUJO_H_INCLUDED
#include "GBT/gbt.h"
#include <stdio.h>
#include <stdlib.h>
#include "juego.h"
#define PIXELES_X_LADO 8
#define PX_PADDING 4

void dibujar(const uint8_t dibujo[][PIXELES_X_LADO], uint16_t oX, uint16_t oY);
void dibujar_bloque(int x, int y, int ancho, int alto, int color) ;
void escribir_texto(int x, int y, const char* texto, int color);
void dibujarmapa(tJuego *juego,int** tablero,int ancho,int alto);
void dibujar_marco(int x, int y , int ancho,int alto , int color);
void dibujarPieza(tPieza *p,int ancho , int alto);
//void dibujar_bloque_tetris(int x, int y, int tam, int color);
void dibujar_bloque_tetris(int x, int y, int ancho,int alto, int color);
void escribir_texto_escalado(int x, int y, const char *texto, int color, int escala);

#endif // DIBUJO_H_INCLUDED
