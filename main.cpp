#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "shaft_speed.h"

unsigned long loops;

bool debug_loop(repeating_timer_t *rt){

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "KHz > %d | RPM > %d"
    , loops / 250, shaft_speed_get_rpm());

    loops = 0;

    return true;
}

void core1() {
    gpio_init(3);
    gpio_set_dir(3, GPIO_OUT);
    
    //Generates a frequency for the rpm functions to read
    while (true){
        bool led_state = !led_state;
        gpio_put(3, led_state);
        sleep_us(600);
        led_state = !led_state;
        gpio_put(3, led_state);
        sleep_us(600);

    }
}

int main() {

    stdio_init_all();

    multicore_launch_core1(core1);

    shaft_speed_init(2, 0, 20);

    static repeating_timer_t timer;
    add_repeating_timer_ms(250, &debug_loop, NULL, &timer);

    while(true){
        loops++;
    }
    return 0;
}