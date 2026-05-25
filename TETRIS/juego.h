#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "GBT/gbt.h"
#define JUGANDO 0
#define MENU 5
#define CONFIG 1
#define RANK 2
#define SALIR 3
#define CANT_OPCIONES 5

typedef struct
{
    int opcionMenu;
    int instancia;
    int ancho_v;
    int alto_v;
    int puntaje;
    int nivel;
    int lineas;
    int piezas_caidas;
    float velocidad_actual;
    //tPieza *pieza;

}tJuego;
/*
typedef struct
{
    int* sec;
    int* psec;
    tRanking ranking;
    int estado;
    int modoJuego;

} tPartida;
*/
typedef struct
{
    int num; //0, 1, 2, 3, 4, 5, 6, 7
    int color;
    int forma[4][4];
    int px;
    int py;
    int tam;           // Tamaño de su matriz cuadrada (2, 3 o 4)
    int **matriz;
}tPieza;

void crearPieza(tPieza *p);
void renderMenu(tJuego *juego);
//void iniciarJuego(tJuego *juego, int** tablero);
void generarNuevoTetramino(tPieza *p);
void iniciarJuego(tJuego *juego, int** tablero);
void rotar_pieza(tPieza *p,int** tablero);
int comprobar_colision(int x, int y, int **matriz, int tam, int **tablero);
void fijar_pieza(tPieza *p, int **tablero);
int verificar_y_limpiar_lineas_punteros(int **tablero, int ancho, int alto, tJuego *juego);
#endif // JUEGO_H_INCLUDED

