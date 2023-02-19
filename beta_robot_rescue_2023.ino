/*  TETRIX Tele-Op Control Module example code for using a genuine Sony PS4 gaming controller to control the TaskBot from the PRIZM Programming Guide
 *  Date: 11/14/2018
 *  Author: ABL
 *  
 *  This example shows how the TaskBot can be controlled with a variety of preprogrammed tasks and movements using the Tele-Op module connected to a PS4 gaming controller.
 */

#include <PRIZM.h>                    // TETRIX PRIZM Arduino Library
#include <TELEOP.h>                   // TETRIX Tele-Op Arduino Library

PRIZM prizm;                          // instantiate a PRIZM object “prizm” so we can use its functions
PS4 ps4;                              // instantiate a PS4 object “ps4” so we can use its functions

float highSpeed = 1;                  // variable to set the motor percentage (100%) for turbo mode
float lowSpeed = highSpeed*.15;       // variable to set the motor percentage (15%) for crawl mode
float medSpeed = highSpeed*.35;       // variable to set the motor percentage (35%) for normal mode
float powerMultiplier = medSpeed;     // variable to set the motor speed when in crawl mode, normal mode, or turbo mode
int touchx = 960;                     // variable to store the x position of the touch pad
int pitchRollBtnCombo = 10;           // variable to set the button combo for pitch and roll control mode (L1 and L2)



void setup() {
 prizm.PrizmBegin();                  // intialize the PRIZM controller; wait here for press of green Start button
 ps4.setDeadZone(LEFT,10);            // set the left joystick dead zone range to +/- 10 
 ps4.setDeadZone(RIGHT,10);           // set the right joystick dead zone range to +/- 10 
 prizm.setMotorInvert(2,1);           // invert the direction of DC Motor 2
 prizm.setServoPosition(1,220);       // set the initial position of the flag to down
 prizm.setServoPosition(2,90);        // set the initial position of the gripper
 ps4.setRumble(STOP);
}

void loop() {
  ps4.getPS4();                                                          // get (read) all PS4 button and joystick data values

 

  if (touchx != ps4.Touchpad(TOUCHX)){touchPadControl();}               // determine if the x value of the touch pad has changed and, if so, call the touchPadControl function

  while (ps4.buttons_1 == pitchRollBtnCombo){pitchRollControl();}       // while the L1 and R1 buttons are pressed, call the pitchRollControl function to enter pitch and roll mode

    
  adjustMotorPowers();                                                                  // call the function to change the high, med, and low speed motor powers
    
  prizm.setMotorPowers(ps4.Motor(RY)*powerMultiplier, ps4.Motor(LY)*powerMultiplier);   // if the joysticks are being used, run the motors at the designated power levels 

  if (ps4.Button(L2)==1){prizm.setMotorDegrees(360,270,360,-270);delay(1200);}          // if the L2 button is pressed, execute a 90-degree turn to the left using encoders
  if (ps4.Button(R2)==1){prizm.setMotorDegrees(360,-270,360,270);delay(1200);}          // if the R2 button is pressed, execute a 90-degree turn to the right using encoders
  prizm.resetEncoders();                                                                // reset the encoders for the next turn
  
  if (ps4.Button(CROSS)==1){prizm.setServoPosition(2, 83);delay(100);}                            // if the CROSS button is pressed,lower the arm
  if (ps4.Button(SQUARE)==1){prizm.setServoPosition(2, 90);delay(100);}                           // if the SQUARE button is pressed, stop the motion of the arm  
  if (ps4.Button(TRIANGLE)==1){prizm.setServoPosition(2, 99.5);delay(100);}                          // if the TRIANGLE button is pressed, raise arm
  if (ps4.Button(CIRCLE)==1){prizm.setServoPosition(2, 90);delay(100);}                             // if the CIRCLE button is pressed, stop the motion of the arm
  prizm.resetEncoders();

  if (ps4.buttons_2 == 4){prizm.setServoPosition(1, 220);}                              //if the left cross is pressed, open the gripper
  if (ps4.buttons_2 == 8){prizm.setServoPosition(1, 80);}                               //if the right cross is pressed, close the gripper
}                                                                                       // end the main loop

void adjustMotorPowers(){                                               // called function to change the low, med, and high speed motor powers
  if(ps4.Button(L1)==1){                                                // if the L1 button is pressed... 
    powerMultiplier = lowSpeed;                                         // ...set powerMultiplier to lowSpeed for crawl mode and...
    ps4.setLED(RED);}                                                   // ...set the game pad LED to red to indicate crawl mode
  else if(ps4.Button(R1)==1){                                           // else if the R1 button is pressed... 
    powerMultiplier = highSpeed;                                        // ...set the powerMultiplier to highSpeed for turbo mode and...
    ps4.setLED(GREEN);}                                                 // ...set the game pad LED to green to indicate turbo mode
  else {                                                                // otherwise, the first two conditions are not true, so...
    powerMultiplier = medSpeed;                                         // ...set the powerMultiplier to medSpeed for normal mode and...
    ps4.setLED(YELLOW);}                                                // ...set the game pad LED to yellow to indicate normal mode
}

void touchPadControl(){                                                 // called function to change the motor powers using the touch pad
  touchx = ps4.Touchpad(TOUCHX);                                        // set the touchx variable to the new x value of the touch pad
  highSpeed = map (touchx,0,1920,0,100);                                // change the highSpeed variable by mapping the touchx variable to motor speeds
  highSpeed = highSpeed / 100;                                          // convert the highSpeed variable to a percentage
  medSpeed = highSpeed*.35;                                             // set the new medSpeed as 35% of the highSpeed variable
  lowSpeed = highSpeed*.15;                                             // set the new lowSpeed as 15% of the highSpeed variable

}

void pitchRollControl(){                                                                        // called function for controlling navigation using pitch and roll of game pad
  ps4.getPS4();                                                                                 // get (read) all PS4 button and joystick data values
  ps4.setLED(BLUE);                                                                             // set the game pad LED to flash blue and red to indicate pitch and roll mode
  prizm.setMotorPowers(ps4.Motor(PITCH)+ps4.Motor(ROLL),ps4.Motor(PITCH)-ps4.Motor(ROLL));      // set the motor power based on values of the game pad's gyroscope for pitch and roll
  ps4.setLED(RED);                                                                              // set the game pad LED to flash blue and red to indicate pitch and roll mode
}
