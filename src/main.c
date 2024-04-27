// Código uso de 8-bit DAC1 gpio25 y DAC2 gpio26 del esp32. Valores de 0 a 255.
// IMPORTANTE: para el uso de generador coseno solo se puede usar un dac, pero puede manejar altas frecuencias.
// NOTAS: Seleccinar comando y pulsar "F12" para acceder a información.
// Para comunicación serial debe añadirse en doc platformio.ini "monitor_speed = 115200".
// Link info: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/dac.html
// LIBRERÍAS:
#include <stdio.h>
#include "driver/gpio.h"       //Puertos
#include "freertos/FreeRTOS.h" //Tiempo procesador
#include "freertos/task.h"     //Tiempo ms
#include "driver/dac_cosine.h"
// DEFINICIONES:
// DAC_CHAN_0: gpio25, DAC_CHAN_1: gpio26.
// #define dac2 26 //Descomentar y modificar código para usar DAC2.

// VARIABLES GLOBALES:
uint32_t frec_variable = 200; // Frecuancia manipulable de 140 Hz a 200 KHz.
// Declaración de métodos:
esp_err_t set_dac(void);
// CÓDIGO PRINCIPAL:
void app_main()
{
    while (1)
    {
        for (size_t i = 0; i < 1000; i++) // Varia cada tantos segundos la frecuencia desde 200 hasta 200k.
        {
            frec_variable = 200 * i + 1; // Varía la frecuencia en cada iteración. RESPETAR RANGO DE FRECUENCIA 140 Hz a 200 KHz.
            set_dac();                   // Llama a la funcion DAC.
        }
    }
}

// COMPROBACIÓN DE CORRECTO FUNCIONAMIENTO:
esp_err_t set_dac(void)
{

    // Configuración DAC:
    dac_cosine_config_t cos_cfg = {
        .chan_id = DAC_CHAN_0,    // DAC_CHAN_0 para GPIO 25, DAC1.
        .freq_hz = frec_variable, // 140 Hz a 200 KHz.
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .atten = DAC_COSINE_ATTEN_DB_6, // DAC_COSINE_ATTEN_DB_6 es 1/2 de la amplitud máxima
        .phase = 0,                     // 0 or 180.
        .offset = 65,                   // Recomendado 65 si 1/2 de amplitud.
    };
    dac_cosine_handle_t ret_handle = NULL;
    // dac_cosine_new_channel(&cos_cfg,&ret_handle); //Crea canal con parámetros definidos.
    dac_cosine_new_channel(&cos_cfg, &ret_handle); // Crea canal con parámetros definidos.

    dac_cosine_start(ret_handle);
    vTaskDelay(8000 / portTICK_PERIOD_MS); // Espera ms.
    dac_cosine_stop(ret_handle);           // Detiene la generación de señal.
    dac_cosine_del_channel(ret_handle);    // Borra configuración del dac para luego poder cambiar valores de configuración.

    return ESP_OK;
}