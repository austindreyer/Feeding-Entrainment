/* Sketch for regulating feeding access for entrainment 
 */


#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>
#include <Time.h>
#include <TimeAlarms.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


// Create the motor shield object with default I2C address
// can also create it with a different I2C address for stacking
// Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
// Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

  Adafruit_MotorShield AFMStop = Adafruit_MotorShield();

// Connect a stepper motor with 513 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4) with red wire as ground, orange and
// pink as a pair, blue and yellow as other pair

  Adafruit_StepperMotor *myStepper1 = AFMStop.getStepper(513, 1);
  Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(513, 2);


// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  myStepper1->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {  
  myStepper1->onestep(BACKWARD, DOUBLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  myStepper2->onestep(BACKWARD, DOUBLE);
}
void backwardstep2() {  
  myStepper2->onestep(FORWARD, DOUBLE);
}

// Now we'll wrap the 2 steppers in an AccelStepper object
  AccelStepper stepper1(forwardstep1, backwardstep1);
  AccelStepper stepper2(forwardstep2, backwardstep2);



void setup() 
{
  Serial.begin(9600);
  AFMStop.begin(); // Start the bottom shield
  //AFMStop.begin(); // Start the top shield

  // Need to set the time and date for alarms to work, remember
  // to use military time 0000-2359, last three variables are for 
  // day, month, year (4 digits)
  setTime(14,22,00,1,9,2017);

  // Create alarms that will trigger events at specified times,
  // using military time
      Alarm.alarmRepeat(14,22,03, FeedN);
      //Alarm.alarmRepeat(8,00,00, FeedY);
  // For specific days of the week, use the Alarm.alarmOnce()
  // function, days stipulated by adding "dow" as prefix to 
  // capitalized day of the week (e.g. dowFriday), for each
  // day of the week to do something. The alarm will cause
  // desired function to be run on stipulated day repeatedly

    // Day 1 TRF
    Alarm.alarmOnce(dowThursday,00,00,00,FeedN);
    Alarm.alarmOnce(dowThursday,18,00,00,FeedY);
    Alarm.alarmOnce(dowThursday,22,00,00,FeedN);
    // Day 2 TRF
    Alarm.alarmOnce(dowFriday,18,00,00,FeedY);
    Alarm.alarmOnce(dowFriday,22,00,00,FeedN);
    // Day 3 TRF
    Alarm.alarmOnce(dowSaturday,18,00,00,FeedY);
    Alarm.alarmOnce(dowSaturday,22,00,00,FeedN);
    //Day 4 TRF
    Alarm.alarmOnce(dowSunday,18,00,00,FeedY);
    Alarm.alarmOnce(dowSunday,22,00,00,FeedN);
    //Day 5 TRF
    Alarm.alarmOnce(dowMonday,18,00,00,FeedY);
    Alarm.alarmOnce(dowMonday,22,00,00,FeedN);
    // END TRF
    Alarm.alarmOnce(dowTuesday,00,00,00,FeedY);
    

}

void loop() 
{
  digitalClockDisplay();
  Alarm.delay(1000);
  
  //FeedY();

  //FeedN();

}

// functions to be called when an alarm triggers

void FeedN()
{
  Serial.println("no Feed start");
  digitalClockDisplay();  
  stepper1.setMaxSpeed(65.0);
  stepper1.setAcceleration(150.0);
  stepper1.moveTo(-75); // pos numbers
    
  stepper2.setMaxSpeed(65.0);
  stepper2.setAcceleration(150.0);
  stepper2.moveTo(-75); // positive = clockwise
 
  bool stepper1moving, stepper2moving;
  do {
    stepper1moving = stepper1.run();    //run() returns true when the stepper has not yet reached its target position.
    stepper2moving = stepper2.run();
  }while(stepper1moving || stepper2moving);
}

void FeedY()
{
  Serial.println("Feed start");
  digitalClockDisplay();  
  stepper1.setMaxSpeed(65.0);
  stepper1.setAcceleration(150.0);
  stepper1.moveTo(75);
    
  stepper2.setMaxSpeed(65.0);
  stepper2.setAcceleration(150.0);
  stepper2.moveTo(75);
 
  bool stepper1moving, stepper2moving;
  do {
    stepper1moving = stepper1.run();    //run() returns true when the stepper has not yet reached its target position.
    stepper2moving = stepper2.run();
  }while(stepper1moving || stepper2moving);
}

void PrintTest1()
{
  Serial.println("test");
}

void PrintTest2()
{
  Serial.println("now");
}

void MorningTest()
{
  Serial.println("AM Feed start");
  digitalClockDisplay();
  myStepper1->step(513,FORWARD,DOUBLE);
  myStepper2->step(513,BACKWARD,DOUBLE);
}


void NightTest()
{
  Serial.println("PM Feed stop");
  digitalClockDisplay();
  myStepper1->step(513,BACKWARD,DOUBLE);
  myStepper2->step(513,FORWARD,DOUBLE);
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


// modified 1/30/2017
