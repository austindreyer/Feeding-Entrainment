/* Test sketch to make a time alarm for the turning of axles using 
 *  the accelstepper library
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
  // day, month, year
  setTime(15,40,00,7,31,2018);

  // Create alarms that will trigger events at specified times,
  // using military time

    //Alarm.alarmOnce(dowFriday,00,00,00,Feed);
    //Alarm.alarmOnce(dowFriday,00,00,10,PrintTest1);
  Alarm.alarmRepeat(03,00,00, Feed3);
  Alarm.alarmRepeat(15,00,00, Feed4);

  //Alarm.alarmRepeat(02,00,5, Feed3);

  //Alarm.alarmRepeat(02,00,25, Feed4);

  //Alarm.alarmRepeat(02,00,3, MorningTest);
  //Alarm.alarmRepeat(02,00,15, NightTest);

  // Describe parameters of motor movements 
  //stepper1.setMaxSpeed(500.0);
  //stepper1.setAcceleration(200.0);
  //stepper1.moveTo(1000);

  //myStepper1<-setSpeed(100);
    
  //stepper2.setMaxSpeed(200.0);
  //stepper2.setAcceleration(200.0);
  //stepper2.moveTo(1000);

  //myStepper2->setSpeed(100);
 

}

void loop() 
{
  digitalClockDisplay();
  Alarm.delay(1000);
  
  //Feed3();
  //PrintTest1();
  //Alarm.delay(0);

    //Feed2();
    //stepper1.run();
    //stepper2.run();

  //MorningTest();
}

// functions to be called when an alarm triggers

void Feed()
{
  Serial.println("AM Feed start");
  digitalClockDisplay();  
  stepper1.setMaxSpeed(65.0);
  stepper1.setAcceleration(90.0);
  stepper1.moveTo(200);
    
  stepper2.setMaxSpeed(65.0);
  stepper2.setAcceleration(150.0);
  stepper2.moveTo(200);
  
  stepper1.runSpeedToPosition();
  stepper2.runSpeedToPosition();
}

void Feed2()
{
  
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(500.0);
  stepper1.moveTo(500);
    
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(500.0);
  stepper2.moveTo(500);
  
  stepper1.run();
  stepper2.run();
}


void Feed3()
{
  Serial.println("AM Feed start");
  digitalClockDisplay();  
  stepper1.setMaxSpeed(65.0);
  stepper1.setAcceleration(150.0);
  stepper1.moveTo(85); // pos numbers
    
  stepper2.setMaxSpeed(65.0);
  stepper2.setAcceleration(150.0);
  stepper2.moveTo(85); // positive = clockwise
 
  bool stepper1moving, stepper2moving;
  do {
    stepper1moving = stepper1.run();    //run() returns true when the stepper has not yet reached its target position.
    stepper2moving = stepper2.run();
  }while(stepper1moving || stepper2moving);
}

void Feed4()
{
  Serial.println("PM Feed start");
  digitalClockDisplay();  
  stepper1.setMaxSpeed(65.0);
  stepper1.setAcceleration(150.0);
  stepper1.moveTo(-2);
    
  stepper2.setMaxSpeed(65.0);
  stepper2.setAcceleration(150.0);
  stepper2.moveTo(-2);
 
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
  Serial.println(weekday());
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
