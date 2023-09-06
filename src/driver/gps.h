#ifndef _GPS_H
#define _GPS_H

#include <Arduino.h>
#include <iostream>
#include <string>

#include "app/fsm.h"

void gps_setup(void);
void gps_fetch_location(void);
std::string gps_poll_location(void);

#endif /*_GPS_H*/