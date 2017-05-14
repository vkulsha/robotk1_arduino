#include <SPI.h>
#include "Ultrasonic.h"

Ultrasonic ultrasonic1(34,35);
Ultrasonic ultrasonic2(36,37);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  attachInterrupt(4, getdata, CHANGE);
  
  pinMode(13, OUTPUT);
  
  pinMode(22,OUTPUT);//rel for left hand 2016
  pinMode(23,OUTPUT);//rel for left hand 2016
  pinMode(24,OUTPUT);//rel for right hand 2016
  pinMode(25,OUTPUT);//rel for right hand 2016
  
  pinMode(34,OUTPUT);
  pinMode(35,INPUT);
  pinMode(36,OUTPUT);
  pinMode(37,INPUT);
  
//pwm         rel1    rel2
//13,12,11,10 9,8,7,6 5,4,3,2
//(Timers 3, 4, 5  Oddly do not use the divisors the data sheet says: Below are actual measured values!)
  TCCR0B = (TCCR0B & 0xF8) | 0x01; //pin 4,13  PWM = 31KHz 
  TCCR1B = (TCCR1B & 0xF8) | 0x01; //pin 11,12 PWM = 31KHz 
  TCCR2B = (TCCR2B & 0xF8) | 0x01; //pin 9,10  PWM = 31KHz 
//  TCCR3B = (TCCR3B & 0xF8) | 0x01; //pin 2,3,5 PWM = 31KHz 
//  TCCR4B = (TCCR4B & 0xF8) | 0x01; //pin 6,7,8 PWM = 31KHz 
//  TCCR5B = (TCCR5B & 0xF8) | 0x01; //pin 44,45,46 PWM = 31KHz 

  setupBlueToothConnection();

}

void loop()
{
}
 
void sonar()
{
    long rating1 = ultrasonic1.Ranging(CM);
    long rating2 = ultrasonic2.Ranging(CM);
    String myString = String(rating1) + "," + String(rating2);
    Serial1.println(myString);
    Serial1.flush();
    delay(100);
  
}

void setupBlueToothConnection()
{	
  Serial1.begin(9600);  
  Serial1.print("\r\n+STWMOD=0\r\n");             // set the bluetooth work in slave mode
  Serial1.print("\r\n+STNA=SeeedBTSlave\r\n");    // set the bluetooth name as "SeeedBTSlave"
  Serial1.print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect me
  Serial1.print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
  delay(2000);                                            // This delay is required.
  Serial1.print("\r\n+INQ=1\r\n");                // make the slave bluetooth inquirable
  delay(2000);                                            // This delay is required. 
  Serial1.flush();
}

void getdata() 
{
    if (Serial1.available() == 54 && Serial1.peek() == 0){
    	for (volatile int pin=0; pin<=53; pin++){
    	  byte volatile val = Serial1.read();
              if (pin == 18 || pin == 19 || pin == 34 || pin == 35 || pin == 36 || pin == 37) { continue; }
    
    	  if (pin > 14 || val == 0){
      		digitalWrite(pin, val);
    	  } else if (pin > 1) {
      		if (val > 10) {
      		  analogWrite(pin, val);
      		} else {
      		  /*if (val > 0)*/ val = 215 + val*4;
      		  analogWrite(pin, val);
      		}
    	  }
    	}
      sonar();
    }
}
