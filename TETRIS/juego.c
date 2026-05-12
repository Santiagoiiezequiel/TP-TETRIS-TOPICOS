#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include "dibujo.h"
#include "GBT/gbt.h"

static const int tetrominos[7][4][4] = {
    // I
    {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,1,0,0}
    },
    // O
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
        // L
     {
        {0,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },

       // J
     {
        {0,0,1,0},
        {0,0,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },

       // Z
     {
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },

     // S
     {
        {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0}
    },

     // T
     {
        {0,0,0,0},
        {1,1,1,0},
        {0,1,0,0},
        {0,0,0,0}
    },

    // T, L, J, S, Z...
};

void iniciarJuego(tJuego *juego, int** tablero){
    // 1. Limpiar el tablero (suponiendo FILAS 20 y COLS 10)
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 10; j++) {
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


void crearPieza(tPieza *p)
{
    int tipo = rand()%7;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            p->forma[i][j] = tetrominos[tipo][i][j];
        }
    }

    p->px = 3;
    p->py = 0;
}
