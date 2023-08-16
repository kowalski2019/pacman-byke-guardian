// Buzzer
// buzzer make sounds
/***********************************/

#include "Arduino.h"
#include "piezo.h"

int buzzer = 12;//the pin of the active buzzer
void piezo_setup(void)
{
  pinMode(buzzer,OUTPUT);//initialize the buzzer pin as an output
}
void piezo_loop(void)
{
 unsigned char i; //define a variable
 while(1)
 {
   //output an frequency
   for(i=0;i<80;i++)
   {
    digitalWrite(buzzer,HIGH);
    delay(1);//wait for 1ms
    digitalWrite(buzzer,LOW);
    delay(1);//wait for 1ms
    }
    //output another frequency
     for(i=0;i<100;i++)
      {
        digitalWrite(buzzer,HIGH);
        delay(2);//wait for 2ms
        digitalWrite(buzzer,LOW);
        delay(2);//wait for 2ms
      }
  }
} 
/****************************************/
