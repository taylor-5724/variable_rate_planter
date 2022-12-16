#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "shaft_speed.h"

unsigned long loops;

bool debug_loop(repeating_timer_t *rt){

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "KHz > %d | Motor RPM > %d | Sim RPM > %d "
    , 0, shaft_speed_get_rpm(4), shaft_speed_get_rpm(2));

    loops = 0;

    return true;
}

void core1() {
    gpio_init(3);
    gpio_set_dir(3, GPIO_OUT);
    
    //Generates a frequency for the rpm functions to read
    while (true){

        uint blink_time = 6000;
        bool led_state = !led_state;
        gpio_put(3, led_state);
        sleep_us(blink_time);
        led_state = !led_state;
        gpio_put(3, led_state);
        sleep_us(blink_time);

    }
}

int main() {

    stdio_init_all();

    multicore_launch_core1(core1);

    shaft_speed_init(2, 2, 20);
    shaft_speed_init(4, 0, 20);

    static repeating_timer_t timer;
    add_repeating_timer_ms(1000, &debug_loop, NULL, &timer);

    while(true){
        loops++;
    }
    return 0;
}