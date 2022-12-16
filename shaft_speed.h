#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define PIO0_IRQ_0 7
#define US_IN_MINUTE 60000000 //1M us in a second * 60 seconds in a minute
#define TOTAL_GPIO_PINS 28

uint64_t _const_rpm_divisor[28];

bool _rpm_read[TOTAL_GPIO_PINS];
volatile uint64_t _time_between_triggers[TOTAL_GPIO_PINS];
volatile uint16_t _times_triggered[TOTAL_GPIO_PINS];
volatile uint64_t _trigger_time[TOTAL_GPIO_PINS];


uint16_t _counts_per_turn[TOTAL_GPIO_PINS];

void _shaft_speed_gpio_callback(uint _gpio, uint32_t _event_mask);

//Input how many decimal places of precision you want into shaft_speed_precision. Max 3
void shaft_speed_init(uint8_t gpio_number, uint8_t shaft_speed_precision, uint16_t counts_per_turn){
    gpio_set_irq_enabled_with_callback(gpio_number, GPIO_IRQ_EDGE_RISE, true, &_shaft_speed_gpio_callback);

    _counts_per_turn[gpio_number] = counts_per_turn;

    _const_rpm_divisor[gpio_number] = US_IN_MINUTE;
    for(uint8_t i = 0; i < shaft_speed_precision and i < 3; i++){
        _const_rpm_divisor[gpio_number] = _const_rpm_divisor[gpio_number] * 10;
    }
  
}

//Returns RPM multiplied by 10 to the power of shaft_speed_precision
uint32_t shaft_speed_get_rpm(uint8_t gpio_to_check){

    irq_set_enabled(PIO0_IRQ_0, false); //Disable interupt while reading values from it

    uint32_t rpm = _time_between_triggers[gpio_to_check] / _times_triggered[gpio_to_check];
    uint64_t time_since_last_trigger = to_us_since_boot(get_absolute_time()) - _trigger_time[gpio_to_check];

    if(rpm < time_since_last_trigger and _rpm_read[gpio_to_check]){
        rpm = time_since_last_trigger;
    }
    else {
        _rpm_read[gpio_to_check] = true;
    }
    irq_set_enabled(PIO0_IRQ_0, true); //Enable interupt

    rpm = (_const_rpm_divisor[gpio_to_check] / rpm) / _counts_per_turn[gpio_to_check];

    return rpm;
}

void _shaft_speed_gpio_callback(uint gpio, uint32_t event_mask) {
    static uint64_t prev_trigger_time[TOTAL_GPIO_PINS];

    if(_rpm_read[gpio]){
        _rpm_read[gpio] = false;
        _time_between_triggers[gpio] = 0;
        _times_triggered[gpio] = 0;
    }

    _trigger_time[gpio] = to_us_since_boot(get_absolute_time());
    _time_between_triggers[gpio] = _time_between_triggers[gpio] + (_trigger_time[gpio] - prev_trigger_time[gpio]);
    _times_triggered[gpio]++;
    prev_trigger_time[gpio] = _trigger_time[gpio];
}