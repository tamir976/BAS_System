#include "pico/stdlib.h"
#include <stdio.h>

static int led_status = 0;

int main(){
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    while (1)
    {
        gpio_put(25, 1);
        led_status = gpio_get(25);
        printf("LED ON\n%d", led_status);
        sleep_ms(500);
        gpio_put(25, 0);
        led_status = gpio_get(25);
        printf("LED OFF\n%d", led_status);
        sleep_ms(500);
    }
    
}