# variable_rate_planter

A program built to run on the Raspberry Pi Pico intended to vary the speed of a corn planter drive to change the rate at which seed is applied

Function definitions:

*Function to intitiate the shaft speed functions on a GPIO*

shaft_speed_init(uint8_t gpio_to_init, uint8_t precision_in_decimal_places, uint16_t counts_per_turn)

*Function to get speed of a shaft in RPM*

shaft_speed_get_rpm(uint8_t gpio_to_get)
