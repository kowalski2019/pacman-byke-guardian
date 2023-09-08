#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Self Includes */

#include "driver/gps.h"
#include "driver/piezo.h"
#include "driver/accelerometer.h"
#include "app/fsm.h"

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define STACK_SIZE 4096

StateMachine machine;
AccelerometerData acc_data;
GPSData gps_data;

/* Main Task to be created. */
void mainTask(void *pvParameters)
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreate() below. */
  // configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

  uint32_t print_acc_data = millis();

  transitionState(&machine, STATE_READY);
  for (;;)
  {
    /* Task code goes here. */

    // poll accelerometer data
    accelerometer_poll_data(acc_data);
    // poll GSM data

    switch (machine.current_state)
    {
    case STATE_IDLE: // nothing to do here
      break;
    case STATE_READY:
      handleEvent(&machine, EVENT_BIKE_IS_PARKING);
      break;
    case STATE_ACTIVE:
      // poll GPS Data
      gps_poll_data(gps_data);
      if (gps_data.fix)
      {
        std::string loc = std::to_string(gps_data.latitudeDegrees) + gps_data.lat + ", " + std::to_string(gps_data.longitudeDegrees) + gps_data.lon;
        Serial.print("Location: ");
        Serial.println(loc.c_str());
      }

      handleEvent(&machine, EVENT_LOCK_BIKE);

      break;
    case STATE_BIKE_LOCKED:
      // check if the bike is moving
      if (millis() - print_acc_data >= 2000)
      {
        print_acc_data = millis();
        Serial.print("Accelerometer data: ");
        Serial.print("X: \t");
        Serial.print(acc_data.x_speed);
        Serial.print(", Y: \t");
        Serial.print(acc_data.y_speed);
        Serial.println();
      }
      if (abs(acc_data.x_speed) > 0.50f)
      {
        handleEvent(&machine, EVENT_START_ALARM);
      }
      break;
    case STATE_ALARM:
      // call the Piezo function to araise the alarm
      piezo_loop();
      // send GPS data by GSM
      if (abs(acc_data.x_speed) >= 0.10f && abs(acc_data.x_speed) <= 0.20f)
      {
        handleEvent(&machine, EVENT_STOP_ALARM);
      }
      break;
    default:
      break;
    }
    // vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  gps_setup();
  piezo_setup();
  accelerometer_setup();

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