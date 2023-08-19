
#include "driver/gps.h"
#include "driver/piezo.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

void setup()
{
  gps_setup();
  piezo_setup();
}


void loop() // run over and over again
{
  gps_loop();
}