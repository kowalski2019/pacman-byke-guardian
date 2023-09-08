#ifndef _GPS_H
#define _GPS_H

#include <Adafruit_GPS.h>

#include <Arduino.h>
#include <iostream>
#include <string>

#include "app/fsm.h"

typedef struct {
    nmea_float_t latitudeDegrees;
    char lat;
    nmea_float_t longitudeDegrees;
    char lon;
    bool fix;
}GPSData;

void gps_setup(void);
void gps_fetch_location(void);
void gps_poll_data(GPSData &gps_data);

#endif /*_GPS_H*/