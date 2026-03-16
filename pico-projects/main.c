#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static QueueHandle_t temp_queue = NULL;

void blink_task(void *pvParameters) {
    const int led_pin = 25;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    uint uIValue;
    while (1) {
        gpio_put(led_pin, 1); // Turn LED on
        uIValue = 1;
        xQueueSend(temp_queue, &uIValue, 0U);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 500 ms

        gpio_put(led_pin, 0); // Turn LED off
        uIValue = 0;
        xQueueSend(temp_queue, &uIValue, 0U);
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 500 ms
    }
}
void print_task(void *pvParameters) {
    uint uIReceivedValue;
    while (1) {
        xQueueReceive(temp_queue, &uIReceivedValue, portMAX_DELAY); // Wait for a value from the queue
        if (uIReceivedValue == 1) {
            printf("LED ON\n");
        } else {
            printf("LED OFF\n");
        }
    }
}

int main(void){
    stdio_init_all();
    temp_queue = xQueueCreate(1, sizeof(uint));
    xTaskCreate(blink_task, "BlinkTask", 256, NULL, 1, NULL);
    xTaskCreate(print_task, "PrintTask", 256, NULL, 1, NULL);
    vTaskStartScheduler(); // Start the FreeRTOS scheduler
    while(1){

    }
}