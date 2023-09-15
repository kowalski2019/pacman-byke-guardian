#include "gsm.h"

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "1322";

#define SMS_TARGET "+4915510467386"

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800   // Modem is SIM800
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <TinyGsmClient.h>
#include "utils/debug.h"

// TTGO T-Call pins
#define MODEM_RST 5
#define MODEM_PWKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 17
#define MODEM_RX 16

// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

//HardwareSerial SerialGSM(0);

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

void gsm_setup()
{

  // Set modem reset, enable, power pins
  //pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  //pinMode(MODEM_POWER_ON, OUTPUT);
  //digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  //digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  Serial.println(modem.getModemInfo());
  delay(3000);

  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  Serial.print("Unlocking SIM Status: ");
  Serial.println(modem.getSimStatus());
  if (strlen(simPIN) && modem.getSimStatus() != 3)
  {
    modem.simUnlock(simPIN);
  }
}

void gsm_loop()
{
  // To send an SMS, call modem.sendSMS(SMS_TARGET, smsMessage)
  String smsMessage = "Hello from ESP32!";
  if (modem.sendSMS(SMS_TARGET, smsMessage))
  {
    Serial.println(smsMessage);
    debug_print_positiv();
  }
  else
  {
    Serial.println("SMS failed to send");
    debug_print_negativ();
  }
  delay(1);
}

/*

int _timeout;
String _buffer;
String number = "+4917658141497"; //-> change with your number

void gsm_setup()
{
  // delay(7000); //delay for 7 seconds to make sure the modules get the signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
}
void gsm_loop()
{
  if (Serial.available() > 0)
    switch (Serial.read())
    {
    case 's':
      gsm_SendMessage();
      break;
    case 'r':
      gsm_RecieveMessage();
      break;
    case 'c':
      gsm_callNumber();
      break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
}
void gsm_SendMessage()
{
  Serial.println("Sending Message");
  sim.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
  delay(200);
  Serial.println("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); // Mobile phone number to send message
  delay(200);
  String SMS = "Hello, some one sthole your bicycle";
  sim.println(SMS);
  delay(100);
  sim.println((char)26); // ASCII code of CTRL+Z
  delay(200);
  _buffer = gsm_readSerial();
}
void gsm_RecieveMessage()
{
  Serial.println("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay(200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
  Serial.write("Unread Message done");
}
String gsm_readSerial()
{
  _timeout = 0;
  while (!sim.available() && _timeout < 12000)
  {
    delay(13);
    _timeout++;
  }
  if (sim.available())
  {
    return sim.readString();
  }
}
void gsm_callNumber()
{
  sim.print(F("ATD"));
  sim.print("+491639213116");
  sim.print(F(";\r\n"));
  _buffer = gsm_readSerial();
  Serial.println(_buffer);
}
*/