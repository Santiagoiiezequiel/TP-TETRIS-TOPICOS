#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "GBT/gbt.h"
#include "dibujo.h"
#include "time.h"
#include "juego.h"
#include <string.h>




typedef struct {
    int paleta;           // 0: Neon Cyberpunk, 1: CGA Clásica, 2: Monocromo
    int resolucion;       // 0: CGA (320x200), 1: VGA (640x480)
    float velocidad_caida; // 1.0f (1000ms), 0.7f (700ms), 0.5f (500ms)
} tConfiguracion;


#endif // CONFIGURACION_H_INCLUDED
