#ifndef _ACC_H
#define _ACC_H

typedef struct {
    float x_speed;
    float y_speed;
    float z_speed;
}AccelerometerData;

void accelerometer_setup(void);

void accelerometer_loop(void);

void accelerometer_poll_data(AccelerometerData &acc_data);

#endif /*_ACC_H*/