#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define PIO0_IRQ_0 7
#define US_IN_MINUTE 60000000

uint64_t _const_rpm_divisor;

bool _rpm_read;
volatile uint64_t _time_between_triggers;
volatile uint16_t _times_triggered;
volatile uint64_t _trigger_time;

void _shaft_speed_gpio_callback(uint _gpio, uint32_t _event_mask);

void shaft_speed_init(uint8_t gpio_number, uint8_t shaft_speed_precision, uint16_t counts_per_turn){
    gpio_set_irq_enabled_with_callback(gpio_number, GPIO_IRQ_EDGE_RISE, true, &_shaft_speed_gpio_callback);

    _const_rpm_divisor = US_IN_MINUTE / counts_per_turn;

    for(uint8_t i = 0; i < shaft_speed_precision; i++){
        _const_rpm_divisor = _const_rpm_divisor * 10;
    }

    
}

uint16_t shaft_speed_get_rpm(){

    irq_set_enabled(PIO0_IRQ_0, 0); //Disable interupt while reading values from it

    uint32_t rpm = _time_between_triggers / _times_triggered;
    uint64_t time_since_last_trigger = to_us_since_boot(get_absolute_time()) - _trigger_time;

    if(rpm < time_since_last_trigger and _rpm_read){
        rpm = time_since_last_trigger;
    }
    _rpm_read = 1;
    irq_set_enabled(PIO0_IRQ_0, 1); //Enable interupt

    rpm = (60000000 / rpm) / 20;

    return rpm;
}

void _shaft_speed_gpio_callback(uint _gpio, uint32_t _event_mask) {
    static uint64_t prev_trigger_time;

    if(_rpm_read){
        _rpm_read = 0;
        _time_between_triggers = 0;
        _times_triggered = 0;
    }

    _trigger_time = to_us_since_boot(get_absolute_time());
    _time_between_triggers = _time_between_triggers + (_trigger_time - prev_trigger_time);
    prev_trigger_time = _trigger_time;
    _times_triggered++;
}