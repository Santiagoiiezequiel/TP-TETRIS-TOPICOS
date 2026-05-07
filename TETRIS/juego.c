#include "juego.h"
#include <stdio.h>
#include <stdlib.h>

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

void iniciarJuego(tJuego *juego, tPieza *p)
{

}

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
