#include "pico/stdlib.h"
#include <stdio.h>
#include <stdint.h>
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "hardware/irq.h"

#define MULTICORE_FIFO_IRQ SIO_FIFO_IRQ_NUM(1)

void core1_interrupt_handler(){
    while (multicore_fifo_rvalid())
    {
        uint16_t raw = multicore_fifo_pop_blocking();
        const float conversion_factor = 3.3f / (1 << 12);
        float temp = 27 - (raw * conversion_factor - 0.706) / 0.001721;
        printf("Temperature: %.2f C\n", temp);
    }
    multicore_fifo_clear_irq();
}

void core1_entry(){
    multicore_fifo_clear_irq();
    irq_set_exclusive_handler(MULTICORE_FIFO_IRQ, core1_interrupt_handler);
    irq_set_enabled(MULTICORE_FIFO_IRQ, true);
    while(1){
        tight_loop_contents();
    }
}
int main(void) {
    stdio_init_all();
    multicore_launch_core1(core1_entry);

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while(1){
        uint32_t raw = adc_read();
        multicore_fifo_push_blocking(raw);
        sleep_ms(1000);
    }

    return 0;
}