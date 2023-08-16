#ifndef _GSM_H
#define _GSM_H

//#include <SoftwareSerial.h>
#include <Arduino.h>

void gsm_setup(void);
void gsm_loop(void);
void gsm_SendMessage();
void gsm_RecieveMessage();
String gsm_readSerial();
void gsm_callNumber();

#endif /*_GSM_H*/