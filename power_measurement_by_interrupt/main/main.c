#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

/* This is BL0937 power & voltage/current measurement code */


#define CF_PIN 2     /* kw pin */
#define CF1_PIN 3    /* V or I RMS */
#define SEL_PIN 18   /* SEL pin */

#define I_CONFIG_STATE 1
#define V_CONFIG_STATE 0

typedef struct {
    volatile int cf_pulse;
    volatile int cf1_pulse;
} PulseCount;

typedef struct {
    float energy_consm;
    float voltage;
    float current;
} ConsumptionData;

PulseCount pulse_count;
ConsumptionData consumption_data;

QueueHandle_t interputQueue;

/* These are the interrupt functions*/
 void IRAM_ATTR power_pulse_interrupt_handler(void *args) {
    pulse_count.cf_pulse++;
}

 void IRAM_ATTR v_i_pulse_interrupt_handler(void *args) {
    pulse_count.cf1_pulse++;
}

/* Voltage Current measurement function for RTOS task - VI_Measure_Task */
void voltage_current_measure(int state) {
    if (state == V_CONFIG_STATE) {
        vTaskDelay(pdMS_TO_TICKS(1000U));
        consumption_data.voltage = (float)(pulse_count.cf1_pulse) * 0.125F;
        printf("V pulse: %d voltage: %f V\n", pulse_count.cf1_pulse, consumption_data.voltage);
        pulse_count.cf1_pulse = 0;
        printf("\n");
    }

    if (state == I_CONFIG_STATE) {
        vTaskDelay(pdMS_TO_TICKS(1000U));
        consumption_data.current = (float)(pulse_count.cf1_pulse) * 0.01F;
        printf("I pulse: %d Current: %f A\n", pulse_count.cf1_pulse, consumption_data.current);
        pulse_count.cf1_pulse = 0;
        gpio_set_level(SEL_PIN, V_CONFIG_STATE);
        printf("\n");
    }
}

/* These are the RTOS tasks below */

/* This is the RTOS task 1 - Power_Measure_Task */
void Power_Measure_Task(void *params) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000U));
        printf("power_pulse: %d Power: %f W\n", pulse_count.cf_pulse, pulse_count.cf_pulse * 1.30F);
        printf("\n");
        consumption_data.energy_consm = consumption_data.energy_consm
                + (float)(pulse_count.cf_pulse) * 1.36F / (3600.0F * 1000.0F);
        printf("Energy: %f KWh\n", consumption_data.energy_consm);
        pulse_count.cf_pulse = 0;
        printf("\n");
    }
}


/* This is the RTOS task 2 - VI_Measure_Task below */
void VI_Measure_Task(void *params) {
    while (1) {
        voltage_current_measure(V_CONFIG_STATE);
        gpio_set_direction(SEL_PIN, GPIO_MODE_OUTPUT);
        gpio_set_level(SEL_PIN, I_CONFIG_STATE);
        voltage_current_measure(I_CONFIG_STATE);
    }
}

/* This is my input pin setup function */
void ittr_pin_setup(int pin) {
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_pulldown_en(pin);
    gpio_pullup_dis(pin);
    gpio_set_intr_type(pin, GPIO_INTR_POSEDGE);
}

void app_main() {
    printf("Power Measurement Starting\n");
    ittr_pin_setup(CF_PIN);
    ittr_pin_setup(CF1_PIN);

    pulse_count.cf_pulse = 0;
    pulse_count.cf1_pulse = 0;

    consumption_data.voltage = 0.0F;
    consumption_data.current = 0.0F;
    consumption_data.energy_consm = 0.0F;

    interputQueue = xQueueCreate(10U, sizeof(int));

    gpio_install_isr_service(0);
    gpio_isr_handler_add(CF_PIN, power_pulse_interrupt_handler, NULL);
    gpio_isr_handler_add(CF1_PIN, v_i_pulse_interrupt_handler, NULL);

    xTaskCreate(Power_Measure_Task, "Power_Measure_Task", 1024U, NULL, 1, NULL);
    xTaskCreate(VI_Measure_Task,    "V_I_Measure_Task",   1024U, NULL, 1, NULL);
}
