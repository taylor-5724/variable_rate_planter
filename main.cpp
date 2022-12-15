#include <stdio.h>
#include "pico/stdlib.h"

#include "shaft_speed.h"

unsigned long loops;

bool debug_loop(repeating_timer_t *rt){

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
    "KHz > %d | RPM > %d"
    , (loops * 4) / 1000, shaft_speed_get_rpm());

    loops = 0;

    return true;
}

int main() {

    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    shaft_speed_init(2, 2, 20);

    static repeating_timer_t timer;
    add_repeating_timer_ms(250, &debug_loop, NULL, &timer);

    while(true){
        loops++;
    }
    return 0;
}