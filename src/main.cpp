
#include "driver/gps.h"
#include "driver/piezo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void setup()
{
  gps_setup();
  piezo_setup();
}

void loop() // run over and over again
{
  gps_loop();
}