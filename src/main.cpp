
#include "driver/gps.h"
#include "driver/piezo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define STACK_SIZE  2048

/* GPS-Tracker Task to be created. */
void vTaskGPSTracker( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */
    //configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
        vTaskDelay( pdMS_TO_TICKS(500) );
        Serial.println("Task GPS-Tracker");
    }
}

/* Piezo ( Alarm ) Task to be created. */
void vTaskPiezo( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. */
    //configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
        //vTaskDelay()
        vTaskDelay( pdMS_TO_TICKS(500) );
        Serial.println("Task Piezo");
    }
}

void setup()
{
  gps_setup();
  piezo_setup();

  Serial.begin(115200);
  
  BaseType_t xReturnedGPSTracker;
  TaskHandle_t xHandleGPSTracker = NULL;

  BaseType_t xReturnedPiezo;
  TaskHandle_t xHandlePiezo = NULL;

    
  /* Create the task, storing the handle. */
  xReturnedGPSTracker = xTaskCreate(
                    vTaskGPSTracker,       /* Function that implements the task. */
                    "GPS-Tracker",          /* Text name for the task. */
                    STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &xHandleGPSTracker );      /* Used to pass out the created task's handle. */

  if( xReturnedGPSTracker == pdPASS )
  {
      /* The task was created.  Use the task's handle to delete the task. */
      vTaskDelete(xHandleGPSTracker);
  }

        /* Create the task, storing the handle. */
  xReturnedPiezo = xTaskCreate(
                    vTaskPiezo,       /* Function that implements the task. */
                    "GPS-Tracker",          /* Text name for the task. */
                    STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &xHandlePiezo);      /* Used to pass out the created task's handle. */

  if( xReturnedPiezo == pdPASS )
  {
      /* The task was created.  Use the task's handle to delete the task. */
    vTaskDelete(xHandlePiezo);
  }

        /* Start the scheduler. */
  Serial.println("Starting scheduler...nr");
  vTaskStartScheduler();

  /* Should never get there */
  Serial.println("Error. Scheduler exited.nr");
  for( ;; );
}

void loop()
{
  Serial.println("Error. Scheduler exited.nr");
  exit(1);
}