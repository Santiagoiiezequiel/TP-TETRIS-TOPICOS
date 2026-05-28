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

void renderPantallaPausa(tJuego *juego)
{
    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int centroX = juego->ancho_v / 2;
    int centroY = juego->alto_v / 2;
    int multAncho = 6 * escala;

    // Caja negra central con borde Magenta (13) para romper la vista del pozo
    int anchoCaja = 140 * escala;
    int altoCaja = 70 * escala;
    dibujar_bloque(centroX - (anchoCaja/2), centroY - (altoCaja/2), anchoCaja, altoCaja, 13);
    dibujar_bloque(centroX - (anchoCaja/2) + (2*escala), centroY - (altoCaja/2) + (2*escala), anchoCaja - (4*escala), altoCaja - (4*escala), 0);

    // Texto PAUSA
    char *txtPausa = "PAUSA";
    escribir_texto_escalado(centroX - ((strlen(txtPausa)*multAncho)/2), centroY - (25*escala), txtPausa, 11, escala); // Cian

    // Opciones
    char *opt1 = "CONTINUAR";
    char *opt2 = "SALIR AL MENU";

    // Condicionales de resalte (reutilizando juego->opcionMenu de forma local temporal)
    escribir_texto_escalado(centroX - ((strlen(opt1)*multAncho)/2), centroY, opt1, (juego->opcionMenu == 0) ? 14 : 7, escala);
    escribir_texto_escalado(centroX - ((strlen(opt2)*multAncho)/2), centroY + (15*escala), opt2, (juego->opcionMenu == 1) ? 14 : 7, escala);
}

void renderPantallaGameOver(tJuego *juego)
{
    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int centroX = juego->ancho_v / 2;
    int centroY = juego->alto_v / 2;
    int multAncho = 6 * escala;

    // Caja contenedora con borde Rojo Brillante (12)
    int anchoCaja = 160 * escala;
    int altoCaja = 80 * escala;
    dibujar_bloque(centroX - (anchoCaja/2), centroY - (altoCaja/2), anchoCaja, altoCaja, 12);
    dibujar_bloque(centroX - (anchoCaja/2) + (2*escala), centroY - (altoCaja/2) + (2*escala), anchoCaja - (4*escala), altoCaja - (4*escala), 0);

    char *txtG = "GAME OVER";
    escribir_texto_escalado(centroX - ((strlen(txtG)*multAncho)/2), centroY - (30*escala), txtG, 12, escala); // Rojo

    char *opt1 = "REINTENTAR";
    char *opt2 = "MENU PRINCIPAL";

    escribir_texto_escalado(centroX - ((strlen(opt1)*multAncho)/2), centroY - (5*escala), opt1, (juego->opcionMenu == 0) ? 10 : 7, escala); // Verde el reintento
    escribir_texto_escalado(centroX - ((strlen(opt2)*multAncho)/2), centroY + (12*escala), opt2, (juego->opcionMenu == 1) ? 14 : 7, escala);
}

void renderPantallaConfig(tJuego *juego)
{
    // 1. DETERMINAMOS LA ESCALA LÓGICA SEGÚN LO QUE EL USUARIO ELIGIÓ EN EL MENÚ EN ESE MOMENTO
    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int multiplicadorAnchoLetra = 6 * escala;

    // 2. RECALCULAMOS EL ANCHO Y ALTO VIRTUAL TEMPORAL PARA QUE EL CENTRADO NO SE ROMPA
    int ancho_virtual = (juego->config.resolucion == 0) ? 320 : 640;
    int alto_virtual  = (juego->config.resolucion == 0) ? 200 : 480;
    int centroX = ancho_virtual / 2;

    // Limpieza absoluta del buffer antes de dibujar
    gbt_borrar_backbuffer(0);

    // =====================================================
    // MARCO GENERAL ADAPTATIVO (Usa las medidas locales dinámicas)
    // =====================================================
    dibujar_bloque(10 * escala, 10 * escala, ancho_virtual - (20 * escala), 1 * escala, 8);
    dibujar_bloque(10 * escala, alto_virtual - (10 * escala), ancho_virtual - (20 * escala), 1 * escala, 8);

    // =====================================================
    // TÍTULO CONFIGURACIÓN (Centrado)
    // =====================================================
    char *titulo = "CONFIGURACION";
    int xTitulo = centroX - ((strlen(titulo) * multiplicadorAnchoLetra) / 2);
    escribir_texto_escalado(xTitulo, 25 * escala, titulo, 14, escala); // Amarillo

    // Un margen izquierdo base para las opciones, centrado geométricamente
    int xOpciones = centroX - (80 * escala);

    // =====================================================
    // 0. PALETA
    // =====================================================
    char strPaleta[30];
    sprintf(strPaleta, "PALETA: < %d >", juego->config.paleta);
    escribir_texto_escalado(xOpciones, 60 * escala, strPaleta, (juego->opcionConfig == 0) ? 11 : 7, escala);

    // =====================================================
    // 1. RESOLUCION
    // =====================================================
    char *resTexto = (juego->config.resolucion == 0) ? "CGA" : "VGA";
    char strRes[30];
    sprintf(strRes, "RESOLUCION: < %s >", resTexto);
    escribir_texto_escalado(xOpciones, 85 * escala, strRes, (juego->opcionConfig == 1) ? 11 : 7, escala);

    // =====================================================
    // 2. VELOCIDAD
    // =====================================================
    char strVel[30];
    sprintf(strVel, "VELOCIDAD BASE: < %.1f >", juego->config.velocidad_caida);
    escribir_texto_escalado(xOpciones, 110 * escala, strVel, (juego->opcionConfig == 2) ? 11 : 7, escala);

    // =====================================================
    // 3. BOTÓN GUARDAR (Centrado independiente)
    // =====================================================
    char *botonVolver = "[ GUARDAR Y VOLVER ]";
    int xBoton = centroX - ((strlen(botonVolver) * multiplicadorAnchoLetra) / 2);
    escribir_texto_escalado(xBoton, 145 * escala, botonVolver, (juego->opcionConfig == 3) ? 10 : 8, escala);
}

void renderPantallaNombre(tJuego *juego)
{
    // Limpieza absoluta con fondo negro
    gbt_borrar_backbuffer(0);

    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int tam_bloque = 8 * escala;
    int multiplicadorAnchoLetra = 6 * escala;
    int centroX = juego->ancho_v / 2;

    // =====================================================
    // MARCO DE LA PANTALLA GENERAL (Marquesina Arcade)
    // =====================================================
    dibujar_bloque(10 * escala, 10 * escala, juego->ancho_v - (20 * escala), 1 * escala, 8);
    dibujar_bloque(10 * escala, juego->alto_v - (10 * escala), juego->ancho_v - (20 * escala), 1 * escala, 8);

    // =====================================================
    // PIEZAS DE TETRIS FLOTANDO ADAPTATIVAS
    // =====================================================
    // Lateral izquierdo: pieza "I" vertical (Cian = 11)
    dibujar_bloque(20 * escala, 40 * escala, 4 * escala, 16 * escala, 11);
    // Lateral derecho: cuadradito "O" (Amarillo = 14)
    dibujar_bloque(juego->ancho_v - (30 * escala), 40 * escala, 8 * escala, 8 * escala, 14);
    // Pieza "T" abajo a la izquierda (Magenta = 13)
    dibujar_bloque(20 * escala, 120 * escala, 12 * escala, 4 * escala, 13);
    dibujar_bloque(24 * escala, 124 * escala, 4 * escala, 4 * escala, 13);

    // =====================================================
    // 1. TITULO "TETRIS" CON EFECTO NEÓN GLOW REAL
    // =====================================================
    int xTetris = centroX - ((strlen("TETRIS") * multiplicadorAnchoLetra) / 2);
    int yTetris = 25 * escala;

    escribir_texto_escalado(xTetris - escala, yTetris,          "TETRIS", 10, escala);
    escribir_texto_escalado(xTetris + escala, yTetris,          "TETRIS", 10, escala);
    escribir_texto_escalado(xTetris,          yTetris - escala, "TETRIS", 10, escala);
    escribir_texto_escalado(xTetris,          yTetris + escala, "TETRIS", 10, escala);
    escribir_texto_escalado(xTetris,          yTetris,          "TETRIS", 16, escala); // Centro blanco

    // =====================================================
    // 2. INDICACIÓN DE NOMBRE CON SOMBRA ARCADE 3D
    // =====================================================
    char *indicacion = "INGRESE NOMBRE";
    int xIndicacion = centroX - ((strlen(indicacion) * multiplicadorAnchoLetra) / 2);
    int yIndicacion = 65 * escala;

    escribir_texto_escalado(xIndicacion + escala, yIndicacion + escala, indicacion, 8, escala);
    escribir_texto_escalado(xIndicacion,           yIndicacion,          indicacion, 11, escala);

    // =====================================================
    // 3. CAJA DE TEXTO ESTILO RETRO CON ESQUINAS ENMARCADAS
    // =====================================================
    int anchoCaja = 120 * escala;
    int xCaja = centroX - (anchoCaja / 2);
    int yCaja = 90 * escala;
    int altoCaja = 18 * escala;

    // Recuadro exterior calado en Magenta Brillante (13)
    dibujar_bloque(xCaja - (2 * escala), yCaja - (2 * escala), anchoCaja + (4 * escala), altoCaja, 13);
    dibujar_bloque(xCaja, yCaja, anchoCaja, altoCaja - (4 * escala), 0); // Vaciado interior negro

    // Corchetes decorativos neón " [  ] " adaptados
    escribir_texto_escalado(xCaja - (12 * escala), yCaja + (3 * escala), "[", 13, escala);
    escribir_texto_escalado(xCaja + anchoCaja + (4 * escala), yCaja + (3 * escala), "]", 13, escala);

    // =====================================================
    // 4. TEXTO DEL JUGADOR DINÁMICO
    // =====================================================
    if (juego->letras_ingresadas == 0)
    {
        int xTexto = centroX - (multiplicadorAnchoLetra / 2);
        escribir_texto_escalado(xTexto, yCaja + (4 * escala), "_", 16, escala);
    }
    else
    {
        int anchoNombre = juego->letras_ingresadas * multiplicadorAnchoLetra;
        int xTexto = centroX - (anchoNombre / 2);
        escribir_texto_escalado(xTexto, yCaja + (4 * escala), juego->nombre_jugador, 14, escala);
    }

    // =====================================================
    // 5. MENSAJE INFERIOR INSTITUCIONAL
    // =====================================================
    char *ayuda = "- JUGAR CON ENTER -";
    int xAyuda = centroX - ((strlen(ayuda) * multiplicadorAnchoLetra) / 2);
    int yAyuda = 155 * escala;

    escribir_texto_escalado(xAyuda, yAyuda, ayuda, 10, escala);

    gbt_volcar_backbuffer();
}

void renderPantallaJuego(tJuego *juego, tPieza *piezasiguiente)
{
    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int tam_bloque = 8 * escala;

    int pozo_ancho = 10 * tam_bloque;
    int pozo_alto  = 20 * tam_bloque;

    int offsetX = (juego->ancho_v - pozo_ancho) / 2;
    int offsetY = (juego->alto_v - pozo_alto) / 2;

    // MARCO DEL TABLERO PRINCIPAL
    dibujar_marco(offsetX - 2, offsetY - 2, pozo_ancho + 4, pozo_alto + 4, 5);

    int anchoCajas = 66 * escala;
    int xPanelDer = offsetX + pozo_ancho + (6 * escala);
    int xPanelIzq = offsetX - anchoCajas - (6 * escala);

    // PANEL LATERAL IZQUIERDO: LINES
    int yLines = offsetY + (10 * escala);
    int altoLines = 32 * escala;

    dibujar_marco(xPanelIzq, yLines, anchoCajas, altoLines, 8);
    escribir_texto_escalado(xPanelIzq + (12 * escala), yLines + (5 * escala), "LINES", 14, escala);

    char strLineas[6];
    sprintf(strLineas, "%03d", juego->lineas_limpiadas);
    escribir_texto_escalado(xPanelIzq + (24 * escala), yLines + (18 * escala), strLineas, 7, escala);

    // PANEL LATERAL DERECHO SUPERIOR: NEXT
    int yNext = offsetY + (10 * escala);
    int altoNext = 50 * escala;

    dibujar_marco(xPanelDer, yNext, anchoCajas, altoNext, 11);
    escribir_texto_escalado(xPanelDer + (18 * escala), yNext + (5 * escala), "NEXT", 11, escala);

    if (piezasiguiente != NULL && piezasiguiente->matriz != NULL)
    {
        int xCentroNext = xPanelDer + (anchoCajas / 2);
        int yCentroNext = yNext + (28 * escala);

        for (int i = 0; i < piezasiguiente->tam; i++)
        {
            for (int j = 0; j < piezasiguiente->tam; j++)
            {
                if (piezasiguiente->matriz[i][j] != 0)
                {
                    int xBloque = xCentroNext + (j - piezasiguiente->tam / 2.0f) * tam_bloque;
                    int yBloque = yCentroNext + (i - piezasiguiente->tam / 2.0f) * tam_bloque;
                    dibujar_bloque_tetris(xBloque, yBloque, tam_bloque, tam_bloque, piezasiguiente->color);
                }
            }
        }
    }

    // PANEL LATERAL DERECHO INFERIOR: SCORE & LEVEL
    int yScore = yNext + altoNext + (8 * escala);
    int altoScore = 34 * escala;

    dibujar_marco(xPanelDer, yScore, anchoCajas, altoScore, 8);
    escribir_texto_escalado(xPanelDer + (15 * escala), yScore + (5 * escala), "SCORE", 10, escala);

    char strScore[8];
    sprintf(strScore, "%06d", juego->puntaje);
    escribir_texto_escalado(xPanelDer + (12 * escala), yScore + (19 * escala), strScore, 7, escala);

    // PANEL LEVEL
    int yLevel = yScore + altoScore + (8 * escala);
    int altoLevel = 34 * escala;

    dibujar_marco(xPanelDer, yLevel, anchoCajas, altoLevel, 8);
    escribir_texto_escalado(xPanelDer + (15 * escala), yLevel + (5 * escala), "LEVEL", 12, escala);

    char strLevel[4];
    sprintf(strLevel, "%02d", juego->nivel);
    escribir_texto_escalado(xPanelDer + (24 * escala), yLevel + (19 * escala), strLevel, 7, escala);
}

void renderMenu(tJuego *juego)
{
    gbt_borrar_backbuffer(0);

    int escala = (juego->config.resolucion == 0) ? 1 : 2;
    int multiplicadorAnchoLetra = 6 * escala;
    int centroX = juego->ancho_v / 2;

    // =====================================================
    // MARCO GENERAL ADAPTATIVO
    // =====================================================
    dibujar_bloque(10 * escala, 10 * escala, juego->ancho_v - (20 * escala), 1 * escala, 8);
    dibujar_bloque(10 * escala, juego->alto_v - (10 * escala), juego->ancho_v - (20 * escala), 1 * escala, 8);

    // =====================================================
    // PIEZAS DE TETRIS DECORATIVAS EN LOS COSTADOS
    // =====================================================
    dibujar_bloque(20 * escala, 60 * escala, 8 * escala, 4 * escala, 12);
    dibujar_bloque(24 * escala, 64 * escala, 8 * escala, 4 * escala, 12);

    dibujar_bloque(juego->ancho_v - (24 * escala), 60 * escala, 4 * escala, 12 * escala, 15);
    dibujar_bloque(juego->ancho_v - (28 * escala), 68 * escala, 8 * escala, 4 * escala, 15);

    // =====================================================
    // 1. TÍTULO GIGANTE "TETRIS" (Escala base 3 * escala de pantalla)
    // =====================================================
    int escalaTitulo = 3 * escala;
    int anchoLetraEscalada = 6 * escalaTitulo;
    int anchoTetrisEscalado = strlen("TETRIS") * anchoLetraEscalada;
    int xTituloInicial = centroX - (anchoTetrisEscalado / 2);
    int yTitulo = 25 * escala;

    int coloresTetris[] = {13, 9, 12, 15, 11, 10};
    char *letrasTetris[] = {"T", "E", "T", "R", "I", "S"};

    for (int l = 0; l < 6; l++)
    {
        int xLetra = xTituloInicial + (l * anchoLetraEscalada);
        escribir_texto_escalado(xLetra + escala, yTitulo + escala, letrasTetris[l], 8, escalaTitulo);
        escribir_texto_escalado(xLetra, yTitulo, letrasTetris[l], coloresTetris[l], escalaTitulo);
    }

    // =====================================================
    // 2. OPCIONES DEL MENÚ TOTALMENTE ESCALABLES
    // =====================================================
    char *nombres[] = {"JUGAR", "CONFIG", "RANKING", "SALIR"};

    for(int i = 0; i < 4; i++)
    {
        int y_pos = (90 * escala) + (i * 24 * escala);

        int largoTexto = strlen(nombres[i]);
        int anchoTextoBoton = largoTexto * multiplicadorAnchoLetra;
        int anchoCaja = anchoTextoBoton + (16 * escala);
        int altoCaja = 13 * escala;
        int xCaja = centroX - (anchoCaja / 2);

        if (i == juego->opcionMenu)
        {
            // Resaltado completo
            dibujar_bloque(xCaja, y_pos, anchoCaja, altoCaja, 14);

            int xTexto = centroX - (anchoTextoBoton / 2);
            escribir_texto_escalado(xTexto, y_pos + (4 * escala), nombres[i], 0, escala);

            // Indicadores de selección "> <"
            escribir_texto_escalado(xCaja - (10 * escala), y_pos + (4 * escala), ">", 10, escala);
            escribir_texto_escalado(xCaja + anchoCaja + (4 * escala), y_pos + (4 * escala), "<", 10, escala);
        }
        else
        {
            // Opción sin seleccionar
            dibujar_bloque(xCaja, y_pos, anchoCaja, altoCaja, 8);
            dibujar_bloque(xCaja + escala, y_pos + escala, anchoCaja - (2 * escala), altoCaja - (2 * escala), 0);

            int xTexto = centroX - (anchoTextoBoton / 2);
            escribir_texto_escalado(xTexto, y_pos + (4 * escala), nombres[i], 7, escala);
        }
    }

    gbt_volcar_backbuffer();
}

void actualizar_ingreso_nombre(tJuego *juego, int **tablero, tPieza *pieza, tPieza *piezaSig)
{
    for (int i = GBTK_a; i <= GBTK_z; i++)
    {
        if (gbt_tecla_presionada(i))
        {
            if (juego->letras_ingresadas < 12)
            {
                char letra = 'A' + (i - GBTK_a);
                juego->nombre_jugador[juego->letras_ingresadas] = letra;
                juego->letras_ingresadas++;
                juego->nombre_jugador[juego->letras_ingresadas] = '\0';
            }
        }
    }

    if (gbt_tecla_presionada(GBTK_RETROCESO))
    {
        if (juego->letras_ingresadas > 0)
        {
            juego->letras_ingresadas--;
            juego->nombre_jugador[juego->letras_ingresadas] = '\0';
        }
    }

    if (gbt_tecla_presionada(GBTK_ENTER))
    {
        if (juego->letras_ingresadas == 0)
        {
            strcpy(juego->nombre_jugador, "PLAYER 1");
        }
        crearPieza(pieza);
        crearPieza(piezaSig);
        juego->instancia = JUGANDO;
    }
}
