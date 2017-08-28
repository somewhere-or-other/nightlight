#include <avr/sleep.h>
#include "nightlight.h"

#define REDPIN 10
#define GREENPIN 11
#define BLUEPIN 9
 
#define FADESPEED 5     // make this higher to slow down

//#define TIMELIMITHOURS 2 //time limit in hours
//#define TIMELIMIT TIMELIMITHOURS*3600*1000 //TIMELIMITHOURS hours * 3600 sec/hr * 1000 millisec/sec


#define TIMELIMIT 120000 //2 minutes in millisec - for testing

float r, g, b;

//float rtarget, gtarget, btarget;

float rdelta, gdelta, bdelta;

int colormax = 255;

unsigned long starttime;



unsigned long elapsedTime(unsigned long a, unsigned long b) {
  if (b<a) {
    //implies an overflow and rollover
    return (b+(0xFFFFFFFF-a));
  } else {
    return (b-a);
  }
}

RGB hsi_to_rgb(const HSI hsi) {
  RGB retrgb;
 
  retrgb = { 0, 0, 0 }; //default value
  
  int hp = (int)(hsi.h * 360 / 60);
  float z = 1 - abs( (hp%2) - 1);
  float c = (3*hsi.i*hsi.s)/(1+z);
  float x = c*z;
  
  if (0<=hp && hp<=1)
    retrgb = {c, x, 0};
  else if (1<=hp && hp<=2)
    retrgb = {x, c, 0};
  else if (2<=hp && hp<=3)
    retrgb = {0, c, x};
  else if (3<=hp && hp<=4)
    retrgb = {0, x, c};
  else if (4<=hp && hp<=5)
    retrgb = {x, 0, c};
  else if (5<=hp && hp<=6)
    retrgb = {c, 0, x};
  
  return retrgb;
}


void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  Serial.begin(9600);
  
  randomSeed(analogRead(0));
  
  
  starttime = millis();

//  for (int c=0; c<=100; c++) {
//    Serial.write("Random number: ");
//    Serial.print(random(256));
//    Serial.write("\n");
//  }
  
  r=0;
  g=0;
  b=0;
  
  analogWrite(REDPIN, (int)r);
  analogWrite(GREENPIN, (int)g);
  analogWrite(BLUEPIN, (int)b);
  
}



void loop() {
  
  //update colormax based on elapsed time
  float elapsedtimeval=elapsedTime(starttime, millis());
  float elapsedtimeratio=elapsedtimeval/TIMELIMIT;
  
//  Serial.write("elapsedtimeval: ");
//  Serial.print(elapsedtimeval);
//  Serial.write("; elapsedtimeratio: ");
//  Serial.print(elapsedtimeratio);
//  Serial.write("\n");
//  
  
  
  colormax = 255 - (int)(256L*elapsedtimeratio);
  if (colormax <= 0)
    colormax = 0; //just for a sanity check


//  Serial.write("colormax: ");
//  Serial.print(colormax);
//  Serial.write("\n");

  if (colormax <= 0) {
    //Serial.write("Sleeping due to running out of time");
    sleepNow();
  } else {
    
    HSI targethsi;
    targethsi.h=((float)random(65000))/65000;
    targethsi.s=((float)random(65000))/65000;
    targethsi.i=colormax*((float)(random(65000))/65000);
    
//    targetrgb.r=random(colormax);
//    targetrgb.g=random(colormax);
//    targetrgb.b=random(colormax);

    RGB targetrgb = hsi_to_rgb(targethsi);


    Serial.write("Target (h, s, i): (");
    Serial.print(targethsi.h);
    Serial.write(", ");
    Serial.print(targethsi.s);
    Serial.write(", ");
    Serial.print(targethsi.i);
    Serial.write(") -> ");
  
    Serial.write("Target (r, g, b): (");
    Serial.print(targetrgb.r);
    Serial.write(", ");
    Serial.print(targetrgb.g);
    Serial.write(", ");
    Serial.print(targetrgb.b);
    Serial.write(")\n");

   
    //color target sanity checks
    if (targetrgb.r <= 0)
      targetrgb.r=0;
    if (targetrgb.g <= 0)
      targetrgb.g=0;
    if (targetrgb.b <= 0)
      targetrgb.b=0;
      
    
    rdelta = targetrgb.r-r;
    gdelta = targetrgb.g-g;
    bdelta = targetrgb.b-b;

    float ardelta = abs(rdelta);
    float agdelta = abs(gdelta);
    float abdelta = abs(bdelta);

    //get max of ardelta, agdelta, abdelta
    int steps = (int)max(max(ardelta, agdelta), abdelta);
	  
    Serial.write("r value: ");
    Serial.print(r);
    Serial.write("; r target: ");
    Serial.print(targetrgb.r);
    Serial.write("; r delta: ");
    Serial.print(rdelta);
    Serial.write("\n");
    
    Serial.write("g value: ");
    Serial.print(g);
    Serial.write("; g target: ");
    Serial.print(targetrgb.g);
    Serial.write("; g delta: ");
    Serial.print(gdelta);
    Serial.write("\n");
    
    Serial.write("b value: ");
    Serial.print(b);
    Serial.write("; b target: ");
    Serial.print(targetrgb.b);
    Serial.write("; b delta: ");
    Serial.print(bdelta);
    Serial.write("\n");
    
    for (int stepcount=0; stepcount <= steps; stepcount++) {
      r += rdelta/steps;
      g += gdelta/steps;
      b += bdelta/steps;
      
      //color level sanity checks
      if (r <= 0)
        r = 0;
      if (g <= 0)
        g = 0;
      if (b <= 0)
        b = 0;
      
      if (stepcount%20 == 0) {
        
        Serial.write("Stepcount: ");
        Serial.print(stepcount);
        Serial.write("; Steps: ");
        Serial.print(steps);
        Serial.write("\n");
        
        Serial.write("Target (r, g, b): (");
        Serial.print((int)r);
        Serial.write(", ");
        Serial.print((int)g);
        Serial.write(", ");
        Serial.print((int)b);
        Serial.write(")\n");
      }
      
      analogWrite(REDPIN, (int)r);
      analogWrite(GREENPIN, (int)g);
      analogWrite(BLUEPIN, (int)b);
      
      delay(FADESPEED*10);
      
    }
  } // end if colormax > 0

}



// sleep code from http://playground.arduino.cc/Learning/ArduinoSleepCode
/* Sleep Demo Serial
 * -----------------
 * Example code to demonstrate the sleep functions in an Arduino.
 *
 * use a resistor between RX and pin2. By default RX is pulled up to 5V
 * therefore, we can use a sequence of Serial data forcing RX to 0, what
 * will make pin2 go LOW activating INT0 external interrupt, bringing
 * the MCU back to life
 *
 * there is also a time counter that will put the MCU to sleep after 10 secs
 *
 * NOTE: when coming back from POWER-DOWN mode, it takes a bit
 *       until the system is functional at 100%!! (typically <1sec)
 *
 * Copyright (C) 2006 MacSimski 2006-12-30
 * Copyright (C) 2007 D. Cuartielles 2007-07-08 - Mexico DF
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
int wakePin = 2;                 // pin used for waking up
int sleepStatus = 0;             // variable to store a request for sleep
int count = 0;                   // counter
 
void wakeUpNow()        // here the interrupt is handled after wakeup
{
  // execute code here after wake-up before returning to the loop() function
  // timers and code using timers (serial.print and more...) will not work here.
  // we don't really need to execute any special functions here, since we
  // just want the thing to wake up
}
 

 
void sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
 
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
 
    /* Now it is time to enable an interrupt. We do it here so an
     * accidentally pushed interrupt button doesn't interrupt
     * our running program. if you want to be able to run
     * interrupt code besides the sleep function, place it in
     * setup() for example.
     *
     * In the function call attachInterrupt(A, B, C)
     * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
     *
     * B   Name of a function you want to execute at interrupt for A.
     *
     * C   Trigger mode of the interrupt pin. can be:
     *             LOW        a low level triggers
     *             CHANGE     a change in level triggers
     *             RISING     a rising edge of a level triggers
     *             FALLING    a falling edge of a level triggers
     *
     * In all but the IDLE sleep modes only LOW can be used.
     */
 
    attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW
 
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
 
}






