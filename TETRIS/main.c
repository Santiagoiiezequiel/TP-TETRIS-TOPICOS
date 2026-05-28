#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GBT/gbt.h"
#include "dibujo.h"
#include "time.h"
#include "juego.h"
#include <string.h>
#include "menu.h"

#define CANT_COLORES 17
#define COL 10
#define FILAS 20

/*
Apellido: Arriola Santiago Ezequiel
DNI: 41743980
Usuario: Santiagoiiezequiel
Entrega: Sí
*/

tGBT_ColorRGB paletaCGA[CANT_COLORES] =
{
    {0x00, 0x00, 0x00}, // 0: Negro
    {0x00, 0x00, 0xAA}, // 1: Azul
    {0x00, 0xAA, 0x00}, // 2: Verde
    {0x00, 0xAA, 0xAA}, // 3: Cian
    {0xAA, 0x00, 0x00}, // 4: Rojo
    {0xAA, 0x00, 0xAA}, // 5: Magenta
    {0xAA, 55, 0x00},   // 6: Marron
    {0xAA, 0xAA, 0xAA}, // 7: Gris claro
    {0x55, 0x55, 0x55}, // 8: Gris oscuro
    {0x55, 0x55, 0xFF}, // 9: Azul brillante
    {0x55, 0xFF, 0x55}, // 10: Verde brillante
    {0x55, 0xFF, 0xFF}, // 11: Cian brillante
    {0xFF, 0x55, 0x55}, // 12: Rojo brillante
    {0xFF, 0x55, 0xFF}, // 13: Magenta brillante
    {0xFF, 0xFF, 0x55}, // 14: Amarillo
    {0xFF, 0x66, 0x00}, // Naranja intenso
    {0xFF, 0xFF, 0xFF}  // 16: Transparente
};

int main(int argc, char *argv[])
{
    tJuego juego;
    tPieza pieza;
    tPieza piezaSiguiente;

    // 1. CARGAMOS LA CONFIGURACIÓN DEL ARCHIVO .TXT ANTES DE INICIAR LA VENTANA
    juego.config.resolucion = 0;
    juego.config.paleta = 0;
    juego.config.velocidad_caida = 1.0f;

    // 2. LEER EL ARCHIVO FÍSICO
    cargarConfiguracion(&juego);

    // 3. CONFIGURAMOS LAS MEDIDAS GEOMÉTRICAS EN BASE A LO QUE SE LEYÓ
    int escala;

    if(juego.config.resolucion == 0)
    {
        // Modo CGA (Baja resolución interna)
        juego.ancho_v = 320;
        juego.alto_v = 200;
        escala = 3; // Ventana física resultante: 960x600
    }
    else
    {
        // Modo VGA (Alta resolución interna)
        juego.ancho_v = 640;
        juego.alto_v = 480;
        escala = 2; // Ventana física resultante: 1280x960 (Modo Gigante)
    }

    // Inicialización del motor gráfico
    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Tetris - %s (%dx%d)", (juego.config.resolucion == 0 ? "CGA" : "VGA"), juego.ancho_v, juego.alto_v);
    if (gbt_crear_ventana(nombreVentana, juego.ancho_v, juego.alto_v, escala) != 0)
    {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    juego.opcionMenu = 0;
    juego.instancia = MENU;

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0)
    {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        return -1;
    }

    int **tablero = malloc(sizeof(int*) * FILAS);
    for(int i = 0; i < FILAS; i++)
    {
        tablero[i] = malloc(sizeof(int) * COL);
        for(int j = 0; j < COL; j++) tablero[i][j] = 0;
    }

    tGBT_Temporizador *temporizador = gbt_temporizador_crear(1.0);
    tGBT_Temporizador *temporizadorGracia = NULL;
    int en_contacto = 0;
    srand(time(0));
    uint8_t corriendo = 1;

    while(corriendo)
    {
        gbt_procesar_entrada();

        // ===================================================
        // ESTADO 1: MENÚ INICIAL
        // ===================================================
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
                switch(juego.opcionMenu)
                {
                case 0: // JUGAR
                    iniciarJuego(&juego, tablero);
                    crearPieza(&pieza);
                    crearPieza(&piezaSiguiente);
                    juego.letras_ingresadas = 0;
                    juego.nombre_jugador[0] = '\0';
                    juego.instancia = INGRESO_NOMBRE;
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

        // ===================================================
        // ESTADO 2: CONFIGURACIÓN
        // ===================================================
        else if(juego.instancia == CONFIG)
        {
            gbt_borrar_backbuffer(0);
            renderPantallaConfig(&juego);
            gbt_volcar_backbuffer();

            if(gbt_tecla_presionada(GBTK_w))
            {
                juego.opcionConfig--;
                if(juego.opcionConfig < 0) juego.opcionConfig = 3;
            }
            if(gbt_tecla_presionada(GBTK_s))
            {
                juego.opcionConfig++;
                if(juego.opcionConfig > 3) juego.opcionConfig = 0;
            }
            if(gbt_tecla_presionada(GBTK_d))
            {
                if(juego.opcionConfig == 0) juego.config.paleta = (juego.config.paleta + 1) % 3;
                if(juego.opcionConfig == 1) juego.config.resolucion = (juego.config.resolucion + 1) % 2;
                if(juego.opcionConfig == 2)
                {
                    juego.config.velocidad_caida++;
                    if(juego.config.velocidad_caida > 3) juego.config.velocidad_caida = 1;
                }
            }
            if(gbt_tecla_presionada(GBTK_a))
            {
                if(juego.opcionConfig == 0)
                {
                    juego.config.paleta--;
                    if(juego.config.paleta < 0) juego.config.paleta = 2;
                }
                if(juego.opcionConfig == 1) juego.config.resolucion = (juego.config.resolucion + 1) % 2;
                if(juego.opcionConfig == 2)
                {
                    juego.config.velocidad_caida--;
                    if(juego.config.velocidad_caida < 1) juego.config.velocidad_caida = 3;
                }
            }

            if(gbt_tecla_presionada(GBTK_ENTER))
            {
                if(juego.opcionConfig == 3) // GUARDAR Y VOLVER
                {
                    // 1. Guardamos los cambios en el archivo físico .txt
                    guardarConfiguracion(&juego);

                    // 2. ACTUALIZAMOS LOS VALORES DE LA ESTRUCTURA EN CALIENTE
                    if (juego.config.resolucion == 0)
                    {
                        juego.ancho_v = 320;
                        juego.alto_v = 200;
                        escala = 3;  // Sincronizado: CGA usa escala 3
                    }
                    else
                    {
                        juego.ancho_v = 640;
                        juego.alto_v = 480;
                        escala = 2;  // Sincronizado: VGA usa escala 2
                    }

                    // 3. DESTRUIMOS LA VENTANA ANTERIOR PARA PERMITIR EL CAMBIO DE TAMAÑO FÍSICO
                    gbt_destruir_ventana();

                    // 4. RE-CREAMOS LA VENTANA GRÁFICA CON LAS NUEVAS MEDIDAS
                    char nombreVentana[128];
                    sprintf(nombreVentana, "Tetris - %s (%dx%d)",
                            (juego.config.resolucion == 0 ? "CGA" : "VGA"),
                            juego.ancho_v, juego.alto_v);

                    gbt_crear_ventana(nombreVentana, juego.ancho_v, juego.alto_v, escala);

                    // 5. RE-APLICAMOS LA PALETA (crear_ventana resetea el contexto gráfico)
                    gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888);

                    // Volvemos al menú principal
                    juego.instancia = MENU;
                }
            }
        }

        // ===================================================
        // ESTADO 3: INGRESO DE NOMBRE
        // ===================================================
        else if (juego.instancia == INGRESO_NOMBRE)
        {
            actualizar_ingreso_nombre(&juego, tablero, &pieza, &piezaSiguiente);
            gbt_borrar_backbuffer(0);
            renderPantallaNombre(&juego);
            gbt_volcar_backbuffer();
        }



        // ===================================================
        // ESTADO 4: EN PARTIDA (JUGANDO)
        // ===================================================
        else if (juego.instancia == JUGANDO)
        {
            int quiere_bajar = 0;
            int quiere_mover_izq = 0;
            int quiere_mover_der = 0;
            int hubo_movimiento_valido = 0;

            if (gbt_temporizador_consumir(temporizador)) quiere_bajar = 1;

            static int framesIzquierda = 0, cooldownIzquierda = 0;
            static int framesDerecha = 0, cooldownDerecha = 0;
            static int bloquearAbajoHastaSoltar = 0, delayAbajo = 0;

            if (gbt_tecla_sostenida(GBTK_IZQUIERDA) || gbt_tecla_presionada(GBTK_IZQUIERDA))
            {
                framesIzquierda++;
                if (gbt_tecla_presionada(GBTK_IZQUIERDA)) quiere_mover_izq = 1;
                else if (framesIzquierda > 100 && ++cooldownIzquierda >= 40)
                {
                    quiere_mover_izq = 1;
                    cooldownIzquierda = 0;
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
                if (gbt_tecla_presionada(GBTK_DERECHA)) quiere_mover_der = 1;
                else if (framesDerecha > 100 && ++cooldownDerecha >= 40)
                {
                    quiere_mover_der = 1;
                    cooldownDerecha = 0;
                }
            }
            else
            {
                framesDerecha = 0;
                cooldownDerecha = 0;
            }

            if (gbt_tecla_sostenida(GBTK_ABAJO) || gbt_tecla_presionada(GBTK_ABAJO))
            {
                if (!bloquearAbajoHastaSoltar)
                {
                    delayAbajo++;
                    if (gbt_tecla_presionada(GBTK_ABAJO)) quiere_bajar = 1;
                    else if (delayAbajo >= 40 && !(comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero)))
                    {
                        quiere_bajar = 1;
                        juego.puntaje += 1;
                        delayAbajo = 0;
                    }
                }
            }
            else
            {
                bloquearAbajoHastaSoltar = 0;
                delayAbajo = 0;
            }

            if (quiere_mover_izq)
            {
                pieza.px--;
                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero)) pieza.px++;
                else hubo_movimiento_valido = 1;
            }
            if (quiere_mover_der)
            {
                pieza.px++;
                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero)) pieza.px--;
                else hubo_movimiento_valido = 1;
            }
            if (gbt_tecla_presionada(GBTK_ARRIBA))
            {
                rotar_pieza(&pieza, tablero);
                hubo_movimiento_valido = 1;
            }

            if (quiere_bajar)
            {
                quiere_bajar = 0;
                pieza.py++;

                if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                {
                    pieza.py--;
                    if (!en_contacto)
                    {
                        en_contacto = 1;
                        temporizadorGracia = gbt_temporizador_crear(juego.velocidad_actual * 0.50f);
                    }
                    if (hubo_movimiento_valido && temporizadorGracia != NULL)
                    {
                        gbt_temporizador_destruir(temporizadorGracia);
                        temporizadorGracia = gbt_temporizador_crear(juego.velocidad_actual * 0.50f);
                    }
                    if (temporizadorGracia != NULL && gbt_temporizador_consumir(temporizadorGracia))
                    {
                        en_contacto = 0;
                        gbt_temporizador_destruir(temporizadorGracia);
                        temporizadorGracia = NULL;

                        fijar_pieza(&pieza, tablero);

                        for (int i = 0; i < pieza.tam; i++) free(pieza.matriz[i]);
                        free(pieza.matriz);

                        int lineas = verificar_y_limpiar_lineas_punteros(tablero, juego.ancho_v, juego.alto_v, &juego);
                        if (lineas > 0)
                        {
                            juego.lineas_limpiadas += lineas;
                            int puntosBase = (lineas == 1) ? 100 : (lineas == 2) ? 300 : (lineas == 3) ? 500 : 800;
                            juego.puntaje += (int)(puntosBase * (1.0f / juego.velocidad_actual));
                        }

                        juego.piezas_caidas++;
                        juego.nivel = (juego.piezas_caidas / 10) + 1;

                        if (juego.piezas_caidas % 10 == 0)
                        {
                            juego.velocidad_actual *= 0.97f;
                            gbt_temporizador_destruir(temporizador);
                            temporizador = gbt_temporizador_crear(juego.velocidad_actual);
                        }

                        bloquearAbajoHastaSoltar = 1;
                        delayAbajo = 0;

                        pieza = piezaSiguiente;
                        crearPieza(&piezaSiguiente);

                        if (comprobar_colision(pieza.px, pieza.py, pieza.matriz, pieza.tam, tablero))
                        {
                            juego.instancia = GAME_OVER;
                        }
                    }
                }
                else if (en_contacto)
                {
                    en_contacto = 0;
                    if (temporizadorGracia != NULL)
                    {
                        gbt_temporizador_destruir(temporizadorGracia);
                        temporizadorGracia = NULL;
                    }
                }
            }

            // ==========================================
            // RENDERIZADO CORREGIDO CON VARIABLES DEL JUEGO
            // ==========================================
            gbt_borrar_backbuffer(0);

            renderPantallaJuego(&juego, &piezaSiguiente);

            dibujarmapa(&juego, tablero, juego.ancho_v, juego.alto_v);
            dibujarPieza(&pieza, juego.ancho_v, juego.alto_v);

            gbt_volcar_backbuffer();
        }

        // ===================================================
        // ESTADO 5: PANTALLA DE PAUSA
        // ===================================================
        else if (juego.instancia == PAUSA)
        {
            // El truco acá es mantener renderizado el fondo del juego estático,
            // borramos el backbuffer pero volvemos a llamar a las funciones del escenario
            gbt_borrar_backbuffer(0);
            renderPantallaJuego(&juego, &piezaSiguiente);
            dibujarmapa(&juego, tablero, juego.ancho_v, juego.alto_v);
            dibujarPieza(&pieza, juego.ancho_v, juego.alto_v);

            // Inyectamos la interfaz modular de Pausa encima de todo
            renderPantallaPausa(&juego);
            gbt_volcar_backbuffer();

            // Controles de la Pausa
            if (gbt_tecla_presionada(GBTK_w) || gbt_tecla_presionada(GBTK_s))
            {
                juego.opcionMenu = (juego.opcionMenu == 0) ? 1 : 0;
            }

            if (gbt_tecla_presionada(GBTK_ESCAPE))
            {
                juego.instancia = JUGANDO; // Salir de pausa directo con ESC
            }

            if (gbt_tecla_presionada(GBTK_ENTER))
            {
                if (juego.opcionMenu == 0) juego.instancia = JUGANDO; // CONTINUAR
                else juego.instancia = MENU;                          // ABANDONAR E IR AL MENU
            }
        }

        // ===================================================
        // ESTADO 6: PANTALLA DE GAME OVER (PERDISTE)
        // ===================================================
        else if (juego.instancia == GAME_OVER)
        {
            gbt_borrar_backbuffer(0);
            // Renderizamos el pozo final muerto opaco de fondo
            renderPantallaJuego(&juego, NULL);
            dibujarmapa(&juego, tablero, juego.ancho_v, juego.alto_v);

            // Dibujamos el cartel neón de Game Over
            renderPantallaGameOver(&juego);
            gbt_volcar_backbuffer();

            if (gbt_tecla_presionada(GBTK_w) || gbt_tecla_presionada(GBTK_s))
            {
                juego.opcionMenu = (juego.opcionMenu == 0) ? 1 : 0;
            }

            if (gbt_tecla_presionada(GBTK_ENTER))
            {
                if (juego.opcionMenu == 0) // VOLVER A JUGAR (Reiniciar estructura)
                {
                    iniciarJuego(&juego, tablero);
                    crearPieza(&pieza);
                    crearPieza(&piezaSiguiente);
                    juego.instancia = JUGANDO;
                }
                else // IR AL MENÚ PRINCIPAL
                {
                    juego.instancia = MENU;
                }
            }

            if (gbt_tecla_presionada(GBTK_ESCAPE))
                juego.instancia = MENU;
        }


        if(gbt_tecla_presionada(GBTK_ESCAPE)) corriendo = 0;
        gbt_esperar(1);
    }

    if (temporizador) gbt_temporizador_destruir(temporizador);
    if (temporizadorGracia) gbt_temporizador_destruir(temporizadorGracia);
    gbt_destruir_ventana();
    for(int i = 0; i < FILAS; i++) free(tablero[i]);
    free(tablero);

    return 0;
}
