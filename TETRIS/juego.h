#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "GBT/gbt.h"
#define JUGANDO 0
#define INGRESO_NOMBRE 6
#define MENU 5
#define CONFIG 1
#define RANK 2
#define SALIR 3
#define CANT_OPCIONES 5
#define PAUSA 7
#define GAME_OVER 10

typedef struct {
    int paleta;           // 0: Neon Cyberpunk, 1: CGA Clásica, 2: Monocromo
    int resolucion;       // 0: CGA (320x200), 1: VGA (640x480)
    float velocidad_caida; // 1.0f (1000ms), 0.7f (700ms), 0.5f (500ms)
} tConfiguracion;

typedef struct
{
    tConfiguracion config;
    int opcionConfig;
    int opcionMenu;
    int instancia;
    int ancho_v;
    int alto_v;
    int puntaje;
    int nivel;
    int lineas;
    int piezas_caidas;
    float velocidad_actual;
    int lineas_limpiadas;
    int letras_ingresadas;
    char nombre_jugador[20];
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
void cargarConfiguracion(tJuego *juego);
void guardarConfiguracion(tJuego *juego);
void generarNuevoTetramino(tPieza *p);
void iniciarJuego(tJuego *juego, int** tablero);
void rotar_pieza(tPieza *p,int** tablero);
int comprobar_colision(int x, int y, int **matriz, int tam, int **tablero);
void fijar_pieza(tPieza *p, int **tablero);
int verificar_y_limpiar_lineas_punteros(int **tablero, int ancho, int alto, tJuego *juego);
int obtenerPiezaAleatoria() ;
#endif // JUEGO_H_INCLUDED

