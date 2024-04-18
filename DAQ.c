#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define READ_DATA_GPIO 34
#define DEFAULT_VREF    1100 // Default reference voltage value (mV)
#define NO_OF_SAMPLES   64   // Number of samples to average 

void app_main() {
    // Initialize ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);

    // Initialize ADC calibration configuration to read calibration value
    esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    while (1) {
        // Read ADC value
        int adc_reading = 0;
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            adc_reading += adc1_get_raw(ADC1_CHANNEL_6);
        }
        adc_reading /= NO_OF_SAMPLES;

        // Convert ADC value to voltage value
        int voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

        printf("%dmV\n", voltage);

        // Wait for 500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // Free ADC calibration configuration memory
    free(adc_chars);
}
