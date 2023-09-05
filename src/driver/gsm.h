#ifndef _GSM_H
#define _GSM_H

//#include <SoftwareSerial.h>
#include <Arduino.h>
#include <iostream>

void gsm_setup(void);
void gsm_loop(void);
void gsm_SendMessage();
void gsm_RecieveMessage();
void gsm_readSerial();
void gsm_callNumber();

#endif /*_GSM_H*/