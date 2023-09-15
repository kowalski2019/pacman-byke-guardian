#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void webserver_setup();
void webserver_loop(QueueHandle_t &msg_queue);

#endif /*_WEBSERVER_H_*/