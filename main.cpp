#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
controller Controller1 = controller(primary);
motor FrontLeftMotor = motor(PORT1, ratio6_1, true);

motor BackLeftMotor = motor(PORT2, ratio6_1, true);

motor MiddleLeftMotor = motor(PORT3, ratio6_1, false);

motor FrontRightMotor = motor(PORT4, ratio6_1, false);

motor BackRightMotor = motor(PORT5, ratio6_1, false);

motor MiddleRightMotor = motor(PORT6, ratio6_1, true);

motor Intake = motor(PORT7, ratio18_1, false);

inertial InertialSensor = inertial(PORT8);

digital_out ArmPneu = digital_out(Brain.ThreeWirePort.A);
digital_out RatchetPneu = digital_out(Brain.ThreeWirePort.B);
digital_out TipPneu = digital_out(Brain.ThreeWirePort.C);
motor slapper = motor(PORT9, ratio18_1, false);




// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;
bool Controller1RightShoulderControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // check the ButtonL1/ButtonL2 status to control Intake
      if (Controller1.ButtonL1.pressing()) {
        Intake.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        Intake.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        Intake.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
      // check the ButtonR1/ButtonR2 status to control slapper
      if (Controller1.ButtonR1.pressing()) {
        slapper.spin(forward);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonR2.pressing()) {
        slapper.spin(reverse);
        Controller1RightShoulderControlMotorsStopped = false;
      } else if (!Controller1RightShoulderControlMotorsStopped) {
        slapper.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1RightShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//                                                                            
//    Project: 344E States Code                                              
//    Author: Henry R
//    Created: 2/28/2024
//    Configuration:        
//                                                                            
// ----------------------------------------------------------------------------


// Includes
#include "vex.h"
#include <string>  
#include <cmath>

// Allows for easier use of the VEX Library
using namespace vex;



// ----------------------------------------------------------------------------
//                                VARIABLE SETUPS
// ----------------------------------------------------------------------------

bool xstate;
bool ystate;
bool bstate;

// ----------------------------------------------------------------------------
//                               FUNCTION SETUPS
// ----------------------------------------------------------------------------


void XPress() {
  xstate = !xstate;
  if (xstate) {
    ArmPneu.set(true);
  } else {
    ArmPneu.set(false);
  }
}

void YPress(){
  ystate = !ystate;
  if (ystate) {
    RatchetPneu.set(true);
    ArmPneu.set(false);
  } else {
    RatchetPneu.set(false);
  }
} 

void BPress(){
  bstate = !bstate;
  if(bstate) {
    TipPneu.set(true);
  } else {
    TipPneu.set(false);
  }
}

// auton selector 
int autonToRun = 0;

// button class - draws buttons and detects clicks
class Button
{
  public:
    int x, y, width, height;
    std::string text;
    color buttonColor, textColor;
    
    Button(int x, int y, int width, int height, std::string text, color buttonColor, color textColor)
    : x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor){}

    void render()
    {
      Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
      Brain.Screen.printAt(x + 15, y + 30, false, text.c_str());
    }

    bool isClicked()
    {
      if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
      Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + width) return true;
      return false;
    }
};

// configures buttons
Button autonButtons[] = {
  Button(10, 10, 150, 50, "DO NOT DO", red, red),
  Button(170, 10, 150, 50, "Right Main", white, black),
  Button(10, 70, 150, 50, "DO NOT DO", red, red),
  Button(170, 70, 150, 50, "Left Main", white, black)
};

// Driving and turning functions

void driveForward(double Distance /* distance in inches divided by wheel circ (10.2102)*/ ) {
  FrontLeftMotor.spinFor(forward, Distance/10.2102, turns, false);
  BackLeftMotor.spinFor(forward, Distance/10.2102, turns, false);
  MiddleLeftMotor.spinFor(forward, Distance/10.2102, turns, false);
  FrontRightMotor.spinFor(forward, Distance/10.2102, turns, false);
  BackRightMotor.spinFor(forward, Distance/10.2102, turns, false);
  MiddleRightMotor.spinFor(forward, Distance/10.2102, turns);

  // waits to return until all motors are stopped moving
  while(FrontLeftMotor.isSpinning() || BackLeftMotor.isSpinning() || MiddleLeftMotor.isSpinning() || 
          FrontRightMotor.isSpinning() || BackRightMotor.isSpinning() || MiddleRightMotor.isSpinning()) {
        wait(10, msec);
    }
}

void driveReverse(double Distance /* distance in inches divided by wheel circ (10.2102)*/ ) {
  FrontLeftMotor.spinFor(reverse, Distance/10.2102, turns, false);
  BackLeftMotor.spinFor(reverse, Distance/10.2102, turns, false);
  MiddleLeftMotor.spinFor(reverse, Distance/10.2102, turns, false);
  FrontRightMotor.spinFor(reverse, Distance/10.2102, turns, false);
  BackRightMotor.spinFor(reverse, Distance/10.2102, turns, false);
  MiddleRightMotor.spinFor(reverse, Distance/10.2102, turns);

  // waits to return until all motors are stopped moving
  while(FrontLeftMotor.isSpinning() || BackLeftMotor.isSpinning() || MiddleLeftMotor.isSpinning() || 
          FrontRightMotor.isSpinning() || BackRightMotor.isSpinning() || MiddleRightMotor.isSpinning()) {
        wait(10, msec);
    }
}

void turnLeft(double Degrees) {
    // Calculate the distance each wheel needs to travel to turn left by the specified number of degrees
    double wheelCircumference = 10.2102; // Wheel circumference in inches
    double distance = (wheelCircumference * Degrees) / 360; // Calculate the distance traveled by each wheel

    // Spin motors for left turn
    FrontLeftMotor.spinFor(reverse, distance, turns, false);
    BackLeftMotor.spinFor(reverse, distance, turns, false);
    MiddleLeftMotor.spinFor(reverse, distance, turns, false);
    FrontRightMotor.spinFor(forward, distance, turns, false);
    BackRightMotor.spinFor(forward, distance, turns, false);
    MiddleRightMotor.spinFor(forward, distance, turns, true);
}

void turnRight(double Degrees) {
    // Calculate the distance each wheel needs to travel to turn right by the specified number of degrees
    double wheelCircumference = 10.2102; // Wheel circumference in inches
    double distance = (wheelCircumference * Degrees) / 360; // Calculate the distance traveled by each wheel

    // Spin motors for right turn
    FrontLeftMotor.spinFor(forward, distance, turns, false);
    BackLeftMotor.spinFor(forward, distance, turns, false);
    MiddleLeftMotor.spinFor(forward, distance, turns, false);
    FrontRightMotor.spinFor(reverse, distance, turns, false);
    BackRightMotor.spinFor(reverse, distance, turns, false);
    MiddleRightMotor.spinFor(reverse, distance, turns, true);
}



bool INtake;
int intakee() {
  
  INtake = true;
  if (INtake){
   Intake.spinFor(forward, 10, turns, true); 
  }
  INtake = false;
  return 0;
}



// ----------------------------------------------------------------------------
//                        COMPETITION FUNCTIONS & CODE
// ----------------------------------------------------------------------------




// ------------
//    AUTON
// ------------
void autonomous()
{
    if(autonToRun == 0) // right rush
    {
      Brain.Screen.clearScreen();
      Brain.Screen.drawImageFromFile("brainbanner.png", 0, 0);
      task intakeTask = task(intakee);
      driveForward(72); // drive to middle triball
      FrontLeftMotor.setVelocity(25, percent);
      BackLeftMotor.setVelocity(25, percent);
      MiddleLeftMotor.setVelocity(25, percent);
      FrontRightMotor.setVelocity(25, percent);
      BackRightMotor.setVelocity(25, percent);
      MiddleRightMotor.setVelocity(25, percent);
      driveForward(12);
      FrontLeftMotor.setVelocity(75, percent);
      BackLeftMotor.setVelocity(75, percent);
      MiddleLeftMotor.setVelocity(75, percent);
      FrontRightMotor.setVelocity(75, percent);
      BackRightMotor.setVelocity(75, percent);
      MiddleRightMotor.setVelocity(75, percent);
      turnRight(75);
      driveForward(40); // drive to goal and deposit triball
      // turnRight(45); // turn to triball on barrier
      // driveForward(49.4772); // drive to triball
      // Intake.spinFor(forward, 10, turns, true); // intake triball
      // turnLeft(180); // turn around
      // driveForward(49.4772); // go to goal and deposit triball
    }

    if(autonToRun == 1) // right main
    {
      Brain.Screen.clearScreen();
      Brain.Screen.drawImageFromFile("brainbanner.png", 0, 0);
      FrontLeftMotor.setStopping(coast);
      BackLeftMotor.setStopping(coast);
      MiddleLeftMotor.setStopping(coast);
      FrontRightMotor.setStopping(coast);
      BackRightMotor.setStopping(coast);
      MiddleRightMotor.setStopping(coast);
    //  driveVelocity(100); // set drive speed to 100%
      driveForward(77); // drive to mid field/goal
      turnRight(60); // turn to goal
      driveForward(30); // drive to deposit
      driveReverse(20);
      // turnRight(110); // turn around to triball at barrier
      // driveForward(65); // drive to triball
      // Intake.spinFor(forward, 3, turns);
      // turnRight(180); // turn around
      // driveForward(62); // go to goal and deposit triball
      // driveReverse(30);
    }

    if(autonToRun == 2)
    {
      Brain.Screen.clearScreen();
      Brain.Screen.drawImageFromFile("brainbanner.png", 0, 0);
     // driveVelocity(100); // set drive speed to 100%
      driveForward(53.666); // drive to middle triball
      Intake.spinFor(forward, 10, turns, true); // intake triball
      turnLeft(45); // turn towards goal
      driveForward(24); // drive to goal and deposit triball
      turnLeft(45); // turn to triball on barrier
      driveForward(49.4772); // drive to triball
      Intake.spinFor(forward, 10, turns, true); // intake triball
      turnRight(180); // turn around
      driveForward(49.4772); // go to goal and deposit triball
    }

    if(autonToRun == 2)
    {
      Brain.Screen.clearScreen();
      Brain.Screen.drawImageFromFile("brainbanner.png", 0, 0);
      FrontLeftMotor.setStopping(coast);
      BackLeftMotor.setStopping(coast);
      MiddleLeftMotor.setStopping(coast);
      FrontRightMotor.setStopping(coast);
      BackRightMotor.setStopping(coast);
      MiddleRightMotor.setStopping(coast);
    //  driveVelocity(100); // set drive speed to 100%
      driveForward(77); // drive to mid field/goal
      turnRight(60); // turn to goal
      driveForward(18); // drive to deposit
      wait (1, seconds);
      driveReverse(30);
      wait(2, seconds);
      turnRight(1509); // turn around to triball at barrier
      driveForward(63); // drive to triball
      Intake.spinFor(forward, 3, turns);
      turnRight(155); // turn around
      driveForward(60); // go to goal and deposit triball
      driveReverse(30);
    }

}

// ------------
//    DRIVER
// ------------
void userControl(void) {

  // Show team banner
  Brain.Screen.drawImageFromFile("brainbanner.png", 0, 0);

  // pneumatics toggles arm 
  Controller1.ButtonX.pressed(XPress);
  // pneumatics toggles ratchets
  Controller1.ButtonY.pressed(YPress);
  // pneumatic toggle anti tip
  Controller1.ButtonA.pressed(BPress);
  
// Driving code
  while (true) {
        // x₁ = Left Motor X = [Controller] [3] position + [Controller] [1] position
        // x₂ = Right Motor X = [Controller] [3] position - [Controller] [1] position
        // Sets each motor's velocity to 0.0001x³ (shown as 0.0001 * x * x * x)
        FrontLeftMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() + Controller1.Axis1.position()) * (Controller1.Axis3.position() + Controller1.Axis1.position())) * (Controller1.Axis3.position() + Controller1.Axis1.position()))), percent);
        BackLeftMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() + Controller1.Axis1.position()) * (Controller1.Axis3.position() + Controller1.Axis1.position())) * (Controller1.Axis3.position() + Controller1.Axis1.position()))), percent);
        MiddleLeftMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() + Controller1.Axis1.position()) * (Controller1.Axis3.position() + Controller1.Axis1.position())) * (Controller1.Axis3.position() + Controller1.Axis1.position()))), percent);
        FrontRightMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() - Controller1.Axis1.position()) * (Controller1.Axis3.position() - Controller1.Axis1.position())) * (Controller1.Axis3.position() - Controller1.Axis1.position()))), percent);
        BackRightMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() - Controller1.Axis1.position()) * (Controller1.Axis3.position() - Controller1.Axis1.position())) * (Controller1.Axis3.position() - Controller1.Axis1.position()))), percent);
        MiddleRightMotor.setVelocity((0.0001 * (((Controller1.Axis3.position() - Controller1.Axis1.position()) * (Controller1.Axis3.position() - Controller1.Axis1.position())) * (Controller1.Axis3.position() - Controller1.Axis1.position()))), percent);
        // Spins motor according to the velocity set
        FrontLeftMotor.spin(forward);
        BackLeftMotor.spin(forward);
        MiddleLeftMotor.spin(forward);
        FrontRightMotor.spin(forward);
        BackRightMotor.spin(forward);
        MiddleRightMotor.spin(forward);

    wait(20, msec);
  }
}

int main()
{
  Intake.setVelocity(100, percent);
  slapper.setVelocity(25, percent);
  FrontLeftMotor.setVelocity(75, percent);
  BackLeftMotor.setVelocity(75, percent);
  MiddleLeftMotor.setVelocity(75, percent);
  FrontRightMotor.setVelocity(75, percent);
  BackRightMotor.setVelocity(75, percent);
  MiddleRightMotor.setVelocity(75, percent);
  FrontLeftMotor.setStopping(coast);
  BackLeftMotor.setStopping(coast);
  MiddleLeftMotor.setStopping(coast);
  FrontRightMotor.setStopping(coast);
  BackRightMotor.setStopping(coast);
  MiddleRightMotor.setStopping(coast);
  Intake.setStopping(coast);

  competition Competition;

  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);

  while(true)
  {
    

    if(!Competition.isEnabled())
    {
      Brain.Screen.clearScreen(white);
      for(int i = 0; i < 4; i++)
      {
        autonButtons[i].render();
        if(autonButtons[i].isClicked())
        {
          autonButtons[autonToRun].buttonColor = white;
          autonButtons[i].buttonColor = green;
          autonToRun = i;
        }
      }
    }

    Brain.Screen.render();
    vex::task::sleep(7);
  }
}
