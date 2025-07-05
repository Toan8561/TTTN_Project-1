#include <Arduino.h>

void setup() 
{
    Serial.begin(9600);//Set the baud rate 9600
}
 
void loop() 
{
    Serial.write("B");   // send A out serial
    delay(2000);   // wait for 2 seconds to send again
}
         