#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

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

}tPieza;

void crearPieza(tPieza *p);
void renderMenu(tJuego *juego);

#endif // JUEGO_H_INCLUDED
