#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define CF_PIN 2     // kw pin
#define CF1_PIN 3   //V or I RMS
#define SEL_PIN 18    //SEL pin

#define  I_CONFIG_STATE 1
#define  V_CONFIG_STATE 0

int vi_pulse = 0;
int power_pulse = 0;


float energy_consm = 0;
float voltage = 0;
float current =0;




QueueHandle_t interputQueue;


volatile int ittr_cf_pulse = 0;
volatile int ittr_cf1_pulse = 0;

static void IRAM_ATTR power_pulse_interrupt_handler(void *args) {

    ittr_cf_pulse++;

}


static void IRAM_ATTR v_i_pulse_interrupt_handler(void *args) {

    ittr_cf1_pulse++;

}


void voltage_current_measure(int state){

	if( state == V_CONFIG_STATE ){

			vTaskDelay(pdMS_TO_TICKS(1000));
			voltage = ittr_cf1_pulse * 0.125;
	        printf("V pulse: %d voltage: %f V\n", ittr_cf1_pulse,voltage);
	        ittr_cf1_pulse = 0;
	        printf("\n");
	}

	if( state == I_CONFIG_STATE ){

				vTaskDelay(pdMS_TO_TICKS(1000));
				current =  ittr_cf1_pulse * 0.01 ;
		        printf("I pulse: %d Current: %f A\n", ittr_cf1_pulse, current);
		        ittr_cf1_pulse = 0;
		        gpio_set_level(SEL_PIN, V_CONFIG_STATE);
		        printf("\n");
		}



}


void Power_Measure_Task(void *params) {

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("power_pulse: %d Power: %f W\n", ittr_cf_pulse, ittr_cf_pulse * 1.30);
        printf("\n");
        energy_consm = energy_consm + ittr_cf_pulse * 1.36 / (3600 * 1000);
        printf("Energy: %f KWh\n", energy_consm);
        ittr_cf_pulse = 0;
        printf("\n");

    }
}




void VI_Measure_Task(void *params) {



    while (1) {
    	voltage_current_measure(V_CONFIG_STATE);
    	gpio_set_direction(SEL_PIN, GPIO_MODE_OUTPUT);
    	gpio_set_level(SEL_PIN, I_CONFIG_STATE);
    	voltage_current_measure(I_CONFIG_STATE);



    }
}














void ittr_pin_setup(int pin){

		gpio_set_direction(pin, GPIO_MODE_INPUT);
	    gpio_pulldown_en(pin);
	    gpio_pullup_dis(pin);
	    gpio_set_intr_type(pin, GPIO_INTR_POSEDGE);

}



void app_main() {

    printf("Power Measurement Starting\n");
    ittr_pin_setup(CF_PIN);
	ittr_pin_setup(CF1_PIN);



    interputQueue = xQueueCreate(10, sizeof(int));



    gpio_install_isr_service(0);
    gpio_isr_handler_add(CF_PIN, power_pulse_interrupt_handler, NULL);
    /*added for experimental purpose*/
    gpio_isr_handler_add(CF1_PIN, v_i_pulse_interrupt_handler, NULL);


    xTaskCreate(Power_Measure_Task, "Power_Measure_Task", 1024, NULL, 1, NULL);
   /*added for experimental purpose*/
    xTaskCreate(VI_Measure_Task, "V_I_Measure_Task", 1024, NULL, 1, NULL);



}
