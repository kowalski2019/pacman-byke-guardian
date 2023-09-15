#include <Arduino.h>
#include "debug.h"

void init_debug(void)
{
    pinMode(18, OUTPUT); // initialize the buzzer pin as an output
    pinMode(19, OUTPUT); // initialize the buzzer pin as an output
    debug_print_positiv();
    delay(2000);
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
}

void debug_print_positiv(void)
{
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
}
void debug_print_negativ(void)
{
    digitalWrite(18, HIGH);
    digitalWrite(19, LOW);
}
