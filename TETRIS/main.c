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
#define CANT_COLORES 17
#define TAM_GRILLA 11
#define COL 10
#define FILAS 20
#define LIMITE_IZQUIERDO 0
#define LIMITE_DERECHO 7

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
    {0xFF, 0x66, 0x00}, // Naranja intenso
    {0xFF, 0xFF, 0xFF}  // 16:  Usado como transparente por GBT

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
    tPieza pieza;
    //tPartida partida;
    //uint16_t offsetX =  0;
    //uint16_t offsetY = 0;

    //int *tablero[FILAS];

    /*
    for(int i = 0; i < FILAS; i++)
    {
        tablero[i] = malloc(sizeof(int) * COL);
        if(!tablero)
            printf("\nNo se pudo reservar memoria ");

        // inicializar en 0
        for(int j = 0; j < COL; j++)
            tablero[i][j] = 0;
    }
    */

    int **tablero;

    tablero = malloc(sizeof(int*) * FILAS);

    for(int i = 0; i < FILAS; i++)
    {
        tablero[i] = malloc(sizeof(int) * COL);

        for(int j = 0; j < COL; j++)
        {
            tablero[i][j] = 0;
        }
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
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    // Guardar en la estructura del juego para usar en los renders
    juego.ancho_v = ancho;
    juego.alto_v = alto;
    juego.opcionMenu = 0;
    juego.instancia = MENU;

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
    juego.instancia=MENU;
    //uint8_t color = rand() % CANT_COLORES;

    while(corriendo)
    {
        gbt_procesar_entrada();

        // ==========================================
        // ESTADO: MENÚ INICIAL
        // ==========================================
        if(juego.instancia == MENU)
        {
            renderMenu(&juego);

            if(gbt_tecla_presionada(GBTK_w))
            {
                juego.opcionMenu--;
                if(juego.opcionMenu < 0) juego.opcionMenu = 3;
            }

            if(gbt_tecla_presionada(GBTK_s))
            {
                juego.opcionMenu++;
                if(juego.opcionMenu > 3) juego.opcionMenu = 0;
            }

            if(gbt_tecla_presionada(GBTK_ENTER))
            {
                printf("ENTER detectado! Opcion actual: %d\n", juego.opcionMenu);
                switch(juego.opcionMenu)
                {
                case 0: // JUGAR
                    // Reiniciamos el tablero a 0 por seguridad
                    for(int i = 0; i < FILAS; i++)
                    {
                        for(int j = 0; j < COL; j++) tablero[i][j] = 0;
                    }
                    juego.puntaje = 0;
                    juego.piezas_caidas = 0;
                    juego.velocidad_actual = 1.0; // Velocidad inicial del temporizador

                    iniciarJuego(&juego, tablero);
                    crearPieza(&pieza);
                    juego.instancia = JUGANDO;
                    break;
                case 1: // CONFIG
                    juego.instancia = CONFIG;
                    break;
                case 3: // SALIR
                    corriendo = 0;
                    break;
                }
            }
        }
        // ==========================================
        // ESTADO: EN PARTIDA (JUGANDO)
        // ==========================================
        else if (juego.instancia == JUGANDO)
        {
            int quiere_bajar = 0;
            int quiere_mover_izq = 0;
            int quiere_mover_der = 0;

            // 1. Gravedad automática normal
            if (gbt_temporizador_consumir(temporizador))
            {
                quiere_bajar = 1;
            }

            // ================================================================
            // 2. SISTEMA DE TECLADO MEJORADO CON RÁFAGA (DAS) Y ANTIFANTASMAS
            // ================================================================

            // Contadores estáticos para el movimiento lateral continuo
            static int framesIzquierda = 0;
            static int cooldownIzquierda = 0;
            static int framesDerecha = 0;
            static int cooldownDerecha = 0;

            // Este flag estático evita que la ráfaga de la pieza anterior afecte a la nueva
            static int bloquearAbajoHastaSoltar = 0;
            static int delayAbajo = 0;

            // --- MOVIMIENTO IZQUIERDA (Poco a poco) ---
            if (gbt_tecla_sostenida(GBTK_IZQUIERDA) || gbt_tecla_presionada(GBTK_IZQUIERDA))
            {
                framesIzquierda++;
                if (gbt_tecla_presionada(GBTK_IZQUIERDA))
                {
                    quiere_mover_izq = 1; // Un toque corto = un casillero
                }
                else
                {
                    cooldownIzquierda++;
                    // Espera 18 frames para arrancar la ráfaga, y luego mueve cada 5 frames (freno real)
                    if (framesIzquierda > 700 && cooldownIzquierda >= 100)
                    {
                        quiere_mover_izq = 1;
                        cooldownIzquierda = 0;
                    }
                }
            }
            else
            {
                framesIzquierda = 0;
                cooldownIzquierda = 0;
            }

            if (gbt_tecla_sostenida(GBTK_DERECHA) || gbt_tecla_presionada(GBTK_DERECHA))
            {
                framesDerecha++;
                if (gbt_tecla_presionada(GBTK_DERECHA))
                {
                    quiere_mover_der = 1; // Un toque corto = un casillero
                }
                else
                {
                    cooldownDerecha++;
                    // Espera 18 frames para arrancar la ráfaga, y luego mueve cada 5 frames
                    if (framesDerecha > 700 && cooldownDerecha >= 100)
                    {
                        quiere_mover_der = 1;
                        cooldownDerecha = 0;
                    }
                }
            }
            else
            {
                framesDerecha = 0;
                cooldownDerecha = 0;
            }

            // --- CONTROL ABAJO ---
            if (gbt_tecla_sostenida(GBTK_ABAJO) || gbt_tecla_presionada(GBTK_ABAJO))
            {
                // Si el escudo está activo, ignoramos la pulsación hasta que levante el dedo
                if (!bloquearAbajoHastaSoltar)
                {
                    delayAbajo++;
                    if (gbt_tecla_presionada(GBTK_ABAJO))
                    {
                        quiere_bajar = 1;
                    }
                    else if (delayAbajo >= 150) // Ráfaga de caída rápida
                    {
                        quiere_bajar = 1;
                        delayAbajo = 0;
                        juego.puntaje += 1;
                    }
                }
            }
            else
            {
                // Cuando el jugador suelta la flecha abajo, desactivamos el escudo protector
                bloquearAbajoHastaSoltar = 0;
                delayAbajo = 0;
            }


            // ================================================================
            // 3. PROCESAMIENTO EFECTIVO DE MOVIMIENTOS Y FÍSICA
            // ================================================================

            // Aplicar movimientos laterales primero
            if (quiere_mover_izq)
            {
                pieza.px--;
                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                {
                    pieza.px++;
                }
            }
            if (quiere_mover_der)
            {
                pieza.px++;
                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                {
                    pieza.px--;
                }
            }

            // Control de rotación estándar
            if (gbt_tecla_presionada(GBTK_ARRIBA))
            {
                rotar_pieza(&pieza, tablero);
            }

            static int framesDeLay = 0;

            if (quiere_bajar)
            {
                quiere_bajar = 0;
                pieza.py++;


                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                {
                    pieza.py--; // Deshacemos el movimiento para dejarla en posición válida

                    framesDeLay++;

                    if (framesDeLay >= 2) // Fui calibrando este numero para ver cuanto tenia para que el usuario mueva
                    {
                        framesDeLay = 0;


                        fijar_pieza(&pieza, tablero);

                        // Liberación de memoria
                        for (int i = 0; i < pieza.tam; i++) {
                            free(pieza.matriz[i]);
                        }
                        free(pieza.matriz);


                        int lineas = verificar_y_limpiar_lineas_punteros(tablero, ancho, alto, &juego);
                        if (lineas > 0)
                        {
                            if (lineas == 1) juego.puntaje += 100;
                            else if (lineas == 2) juego.puntaje += 300;
                            else if (lineas == 3) juego.puntaje += 500;
                            else if (lineas == 4) juego.puntaje += 800;
                        }


                        juego.piezas_caidas++;
                        if (juego.piezas_caidas > 0 && juego.piezas_caidas % 10 == 0)
                        {
                            juego.velocidad_actual *= 0.95f;
                            gbt_temporizador_destruir(temporizador);
                            temporizador = gbt_temporizador_crear(juego.velocidad_actual);
                        }


                        bloquearAbajoHastaSoltar = 1;
                        delayAbajo = 0;


                        crearPieza(&pieza);

                        if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                        {
                            printf("GAME OVER - Tablero lleno.\n");
                            juego.instancia = MENU;
                        }
                    }
                }
                else
                {
                    framesDeLay = 0;
                }
            }


            gbt_borrar_backbuffer(0);
            dibujarmapa(&juego, tablero, ancho, alto);
            dibujarPieza(&pieza, ancho, alto);
            gbt_volcar_backbuffer();

        }

        if(gbt_tecla_presionada(GBTK_ESCAPE))
        {
            corriendo = 0;
        }

    }


    for(int i = 0; i < FILAS; i++)
    {
        free(tablero[i]);
    }
    free(tablero);

    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();
    return 0;

}

