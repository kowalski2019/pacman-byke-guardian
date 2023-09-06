
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Self Includes */

#include "driver/gps.h"
#include "driver/piezo.h"
#include "app/fsm.h"

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define STACK_SIZE 4096

StateMachine machine;

/* Main Task to be created. */
void mainTask(void *pvParameters)
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreate() below. */
  // configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

  std::string gps_location;
  transitionState(&machine, STATE_READY);
  for (;;)
  {
    /* Task code goes here. */

    // poll accelerometer data
    // poll GSM data

    switch (machine.current_state)
    {
    case STATE_IDLE: // nothing to do here
      break;
    case STATE_READY:
      handleEvent(&machine, EVENT_BIKE_IS_PARKING);
      break;
    case STATE_ACTIVE:
      gps_location = gps_poll_location();
      Serial.print("Location: ");
      Serial.println(gps_location.c_str());
      break;
    case STATE_BIKE_LOCKED:
      // check if the bike is moving
      break;
    case STATE_ALARM:
      // call the Piezo function to araise the alarm
      break;
    default:
      break;
    }

    //Serial.println("Task GPS-Tracker");

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  gps_setup();
  piezo_setup();

  initStateMachine(&machine);

  Serial.begin(115200);

  TaskHandle_t xHandleMainTask = NULL;

  /* Create the task, storing the handle. */
  xTaskCreatePinnedToCore(
      mainTask,         // Function to be called
      "GPS-Tracker",    // Name of task
      STACK_SIZE,       // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,             // Parameter to pass to function
      1,                // Task priority (0 to configMAX_PRIORITIES - 1)
      &xHandleMainTask, // Task handle
      app_cpu           // Run on one core
  );
}

void loop()
{
  // leave empty
}