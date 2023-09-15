#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Self Includes */

#include "utils/debug.h"
#include "driver/gps.h"
#include "driver/gsm.h"
#include "driver/piezo.h"
#include "driver/accelerometer.h"

#include "app/fsm.h"
#include "app/webserver.h"

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

static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;

static int x_before_lock;
static int y_before_lock;

// https://www.google.com/maps/place/39.724124908447N,16.516248703003E

void webserverTask(void *pvParameters)
{
  for (;;)
  {
    webserver_loop(msg_queue);
    // Serial.println("Hey task 2");
    // vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
/* Main Task to be created. */
void mainTask(void *pvParameters)
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreate() below. */
  // configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

  uint32_t print_acc_data = millis();
  int bike_lock_statue;

  transitionState(&machine, STATE_READY);
  for (;;)
  {
    /* Task code goes here. */

    if (xQueueReceive(msg_queue, (void *)&bike_lock_statue, 0) == pdTRUE)
    {
      Serial.print("Bike State: ");
      Serial.println(bike_lock_statue);

      xQueueReset(msg_queue);
    }

    // poll GPS Data
    gps_poll_data(gps_data);

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
      if (gps_data.fix)
      {
        std::string loc = std::to_string(gps_data.latitudeDegrees) + gps_data.lat + ", " + std::to_string(gps_data.longitudeDegrees) + gps_data.lon;
        Serial.print("Location: ");
        Serial.println(loc.c_str());
      }

      x_before_lock = acc_data.x;
      y_before_lock = acc_data.y;
      if (bike_lock_statue)
      {
        handleEvent(&machine, EVENT_LOCK_BIKE);
      }

      break;
    case STATE_BIKE_LOCKED:
      // check if the bike is moving
      if (millis() - print_acc_data >= 2000)
      {
        print_acc_data = millis();
        Serial.print("Accelerometer data: ");
        Serial.print("X: \t");
        Serial.print(acc_data.x);
        Serial.print(", Y: \t");
        Serial.print(acc_data.y);
        Serial.println();
      }
      if (gps_data.fix)
      {
        std::string loc = std::to_string(gps_data.latitudeDegrees) + gps_data.lat + ", " + std::to_string(gps_data.longitudeDegrees) + gps_data.lon;
        Serial.print("Location: ");
        Serial.println(loc.c_str());
      }
      // ((abs(acc_data.x) - abs(x_before_lock)) > 10) TODO
      if ((abs(acc_data.x_speed) > 3.0f))
      {
        handleEvent(&machine, EVENT_START_ALARM);
      }
      if (!bike_lock_statue)
      {
        handleEvent(&machine, EVENT_UNLOCK_BIKE);
      }
      break;
    case STATE_ALARM:
      // call the Piezo function to araise the alarm
      piezo_loop();
      // send GPS data by GSM
      if (abs(acc_data.x_speed) < 3.0f)
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

  Serial.begin(115200);

  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
  initStateMachine(&machine);

  init_debug();

  gps_setup();
  piezo_setup();
  accelerometer_setup();

  webserver_setup();

  delay(5000);
  Serial.println("Init GSM now");
  gsm_setup();
  gsm_loop(); // just call the send SMS func to see if it work

  TaskHandle_t xHandleMainTask = NULL;

  /* Create the task, storing the handle. */
  xTaskCreatePinnedToCore(
      mainTask,         // Function to be called
      "MainTask",       // Name of task
      STACK_SIZE,       // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,             // Parameter to pass to function
      2,                // Task priority (0 to configMAX_PRIORITIES - 1)
      &xHandleMainTask, // Task handle
      app_cpu           // Run on one core
  );
  xTaskCreatePinnedToCore(
      webserverTask, // Function to be called
      "Web-Server",  // Name of task
      STACK_SIZE,    // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,          // Parameter to pass to function
      1,             // Task priority (0 to configMAX_PRIORITIES - 1)
      NULL,          // Task handle
      app_cpu        // Run on core 0
  );
}

void loop()
{
  // leave empty
}