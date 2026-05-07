#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GBT/gbt.h"
#include "dibujo.h"
#include "time.h"
#include "juego.h"
#include <string.h>
#define ANCHO_VENTANA 320
#define ALTO_VENTANA 200
#define ESCALA_VENTANA 1
#define CANT_COLORES 16
#define TAM_GRILLA 11
#define COL 2
#define FILAS 2

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{

    /// 0-15: Colores CGA (16 colores)
    {0x00, 0x00, 0x00}, // 0:   Negro
    {0x00, 0x00, 0xAA}, // 1:   Azul
    {0x00, 0xAA, 0x00}, // 2:   Verde
    {0x00, 0xAA, 0xAA}, // 3:   Cian
    {0xAA, 0x00, 0x00}, // 4:   Rojo
    {0xAA, 0x00, 0xAA}, // 5:   Magenta
    {0xAA, 0x55, 0x00}, // 6:   Marron
    {0xAA, 0xAA, 0xAA}, // 7:   Gris claro
    {0x55, 0x55, 0x55}, // 8:   Gris oscuro
    {0x55, 0x55, 0xFF}, // 9:   Azul brillante
    {0x55, 0xFF, 0x55}, // 10:  Verde brillante
    {0x55, 0xFF, 0xFF}, // 11:  Cian brillante
    {0xFF, 0x55, 0x55}, // 12:  Rojo brillante
    {0xFF, 0x55, 0xFF}, // 13:  Magenta brillante
    {0xFF, 0xFF, 0x55}, // 14:  Amarillo
    {0xFF, 0xFF, 0xFF}  // 15:  Usado como transparente por GBT
};


int main(int argc, char *argv[])
{
    // Valores por defecto
    int ancho = 320;
    int alto = 200;
    int escala = 2;

    // Procesar argumentos: ej: ./tetris.exe VGA 1
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "CGA") == 0)
        {
            ancho = 320;
            alto = 200;
        }
        else if (strcmp(argv[i], "VGA") == 0)
        {
            ancho = 640;
            alto = 480;
        }
        else
        {
            // Si es un número, lo tomamos como escala
            int e = atoi(argv[i]);
            if (e > 0) escala = e;
        }
    }

    tJuego juego;
    //tPartida partida;
    uint16_t offsetX =  0;
    uint16_t offsetY = 0;

    int *tablero[FILAS];

    for(int i = 0; i < FILAS; i++)
    {
        tablero[i] = malloc(sizeof(int) * COL);

        // inicializar en 0
        for(int j = 0; j < COL; j++)
            tablero[i][j] = 0;
    }

    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Tetris - %s (%dx%d)", (ancho == 320 ? "CGA" : "VGA"), ancho, alto);
    if (gbt_crear_ventana(nombreVentana, ancho, alto, escala) != 0)
    {
        return -1;
    }

    // Guardar en la estructura del juego para usar en los renders
    juego.ancho_v = ancho;
    juego.alto_v = alto;
    juego.opcionMenu = 0;
    juego.instancia = MENU;

    if (gbt_crear_ventana(nombreVentana, ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0)
    {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0)
    {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        return -1;
    }

    tGBT_Temporizador *temporizador = gbt_temporizador_crear(1.0);
    if (!temporizador)
    {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        return -1;
    }

    srand(time(0));
    uint8_t corriendo= 1;
    int opcionSeleccionada=0;
    juego.instancia=MENU;
    uint8_t color = rand() % CANT_COLORES;

    while(corriendo)
    {
        //if(juego.instancia == MENU)
        //menuInicial(&juego, &partida);

        gbt_procesar_entrada();
        if(gbt_tecla_presionada(GBTK_w))
        {
            juego.opcionMenu--;
            // Si se pasa del mínimo (0), salta al máximo (3)
            if(juego.opcionMenu < 0)
            {
                juego.opcionMenu = 3;
            }
        }

        if(gbt_tecla_presionada(GBTK_s))
        {
            juego.opcionMenu++;
            // Si se pasa del máximo (3), vuelve al inicio (0)
            if(juego.opcionMenu > 3)
            {
                juego.opcionMenu = 0;
            }
        }

        renderMenu(&juego);
        // Suponiendo que tu menú tiene este orden visual:
    // 0: JUGAR, 1: CONFIG, 2: RANK, 3: SALIR
        if(gbt_tecla_presionada(GBTK_ENTER))
        {
            printf("ENTER detectado! Opcion actual: %d\n", juego.opcionMenu);
            switch(juego.opcionMenu)
            {
            case 0: // JUGAR
                juego.instancia = JUGANDO;
                dibujarmapa(&juego,tablero);

                // Aquí podrías resetear el juego o cambiar de pantalla
                break;
            case 1: // CONFIG
                juego.instancia = CONFIG;
                break;
            case 3: // SALIR
                corriendo = 0;
                break;
            }
        }

        /*
        if(juego.instancia == CONFIG)
            menuConfig(&juego, &partida);

        if(juego.instancia == JUGANDO)
        {
            dibujar(&juego, tablero);
        }
        */
        if(gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo=0;
        }
    }


    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();
    return 0;


}

