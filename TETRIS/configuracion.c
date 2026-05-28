#include "configuracion.h"

#define CONFIG_FILE "config.txt"

// FUNCIÓN PARA GUARDAR EN ARCHIVO
void guardarConfiguracion(tConfiguracion *config)
{
    FILE *archivo = fopen(CONFIG_FILE, "w");
    if (archivo == NULL)
    {
        // Si hay un error abriendo el archivo, salimos de la función
        return;
    }

    // Guardamos los tres valores con un formato simple (un número por línea)
    fprintf(archivo, "%d\n", config->paleta);
    fprintf(archivo, "%d\n", config->resolucion);
    fprintf(archivo, "%f\n", config->velocidad_caida);

    fclose(archivo);
}

// FUNCIÓN PARA CARGAR DESDE ARCHIVO
void cargarConfiguracion(tConfiguracion *config)
{
    FILE *archivo = fopen(CONFIG_FILE, "r");

    // Si el archivo NO existe (primera vez que se abre el juego),
    // cargamos valores por defecto seguros:
    if (archivo == NULL)
    {
        config->paleta = 0;          // Paleta clásica
        config->resolucion = 0;      // 0 = CGA (320x200)
        config->velocidad_caida = 1;  // Velocidad inicial estándar

        // Creamos el archivo por defecto para la próxima sesión
        guardarConfiguracion(config);
        return;
    }

    // Si el archivo existe, leemos las variables en el mismo orden en que las guardamos
    fscanf(archivo, "%d", &config->paleta);
    fscanf(archivo, "%d", &config->resolucion);
    fscanf(archivo, "%f", &config->velocidad_caida);

    fclose(archivo);
}
