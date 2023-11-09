//Libraries
/*

Prints time stamps for 5 seconds using getXXX functions

Based on DS3231_set.pde
by Eric Ayars
4/11

Added printing back of time stamps and increased baud rate
(to better synchronize computer and RTC)
Andy Wickert
5/15/2011

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 clock1;

bool century = false;
bool h12Flag;
bool pmFlag;

#include <AccelStepper.h> //ManageLibraries > AccelStepper
//it works now; hardware issue. Have fun! ~~ Vincent

// RAMPS 1.4 pins
//Stepper defines
#define E0_STEP 26 //Digital 26
#define E0_DIR 28
#define E0_EN 24

#define E1_STEP 36
#define E1_DIR 34
#define E1_EN 30

#define X_STEP 2 //A0
#define X_DIR 5 //A1
#define X_EN 8

#define Y_STEP 60 //A6
#define Y_DIR 61 //A7
#define Y_EN 56 //A2

#define Z_STEP 46
#define Z_DIR 48
#define Z_EN 62 //A8?

//AccelStepper setup
//define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, X_STEP, X_DIR); //uses the driver on E0

void setup() {
  // Start the I2C interface
  Wire.begin();
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HydroMouse V0.1");

  pinMode(X_STEP, OUTPUT); //set "step" pin as an output
  pinMode(X_DIR, OUTPUT); //set "direction" pin as an output
  pinMode(X_EN, OUTPUT); //set "enable" pin as an output
  digitalWrite(X_EN, LOW); //enable stepper E0
  stepper.setCurrentPosition(0); //set current position as zero

    for (int i=0; i<5; i++){
      delay(1000);
      Serial.print(clock1.getYear(), DEC);
      Serial.print("-");
      Serial.print(clock1.getMonth(century), DEC);
      Serial.print("-");
      Serial.print(clock1.getDate(), DEC);
      Serial.print(" ");
      Serial.print(clock1.getHour(h12Flag, pmFlag), DEC); //24-hr
      Serial.print(":");
      Serial.print(clock1.getMinute(), DEC);
      Serial.print(":");
      Serial.println(clock1.getSecond(), DEC);
  }
}
 
void loop() { 
  // put your main code here, to run repeatedly:
  setStepperDefaults();

//  Serial.print(int(clock1.getMinute()));
  if(int(clock1.getHour(h12Flag, pmFlag)) == 14 && int(clock1.getMinute())== 36){
     runStepper(23);
  }
  if(int(clock1.getMinute())== 37){
    runStepper(-23);
  }
 
  //delay(1000);
//  Serial.println(stepper.currentPosition());

  
}
 
void setStepperDefaults(){
    stepper.setMaxSpeed(600.0);
    stepper.setAcceleration(100.0);
}
// converts steps to mL
int mlToSteps(int ml){
  return(ml * 400); //it takes 100 steps to move 1ml of water
}

// runs the stepper given direction
void runStepper(int steps) {
  stepper.moveTo(mlToSteps(steps)); //move 200 steps
  stepper.runToPosition();
}
