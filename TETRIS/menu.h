#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GBT/gbt.h"
#include "dibujo.h"
#include "time.h"
#include "juego.h"
#include <string.h>



void renderMenu(tJuego *juego);
void renderPantallaNombre(tJuego *juego);
void renderPantallaConfig(tJuego *juego);
void renderPantallaGameOver(tJuego *juego);
void renderPantallaPausa(tJuego *juego);
void renderPantallaJuego(tJuego *juego,tPieza *piezasiguiente);
void actualizar_ingreso_nombre(tJuego *juego, int **tablero, tPieza *pieza, tPieza *piezaSig);
#endif // MENU_H_INCLUDED

