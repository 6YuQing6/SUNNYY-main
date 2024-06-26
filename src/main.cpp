/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       robol                                                     */
/*    Created:      6/16/2024, 1:25:16 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "functions.h"
#include <config.h>
using namespace vex;

#define RED true
#define BLUE false

#define Alliance RED
#define CloseSide true

#define MANUAL true


brain Brain;
// A global instance of competition
competition Competition;

controller Controller1 = controller(primary);
motor Catapult = motor(PORT2, ratio36_1, true);
motor leftDriveA = motor(PORT4, ratio6_1, true);
motor leftDriveB = motor(PORT8, ratio6_1, true);
motor leftDriveC = motor(PORT10, ratio6_1, true);
motor rightDriveA = motor(PORT14, ratio6_1, false);
motor rightDriveB = motor(PORT18, ratio6_1, false);
motor rightDriveC = motor(PORT20, ratio6_1, false);
motor Intake = motor(PORT15, ratio6_1, true);
const int32_t InertialPort = PORT13;


digital_out Wings = digital_out(Brain.ThreeWirePort.A);
digital_out Hang = digital_out(Brain.ThreeWirePort.C);

motor_group LeftDriveSmart = motor_group(leftDriveA, leftDriveB, leftDriveC);
motor_group RightDriveSmart = motor_group(rightDriveA, rightDriveB, rightDriveC);

// for pid autonomous
Drive Chassis(LeftDriveSmart,RightDriveSmart,InertialPort, 3.25, 0.6, 360);

// for manual autonomous
inertial DriveTrainInertial = inertial(PORT13);
smartdrive DriveTrain = smartdrive(LeftDriveSmart, RightDriveSmart, DriveTrainInertial, 10.21018, 10.8, 12, inches, 0.6); // wheel circumference, track width, wheelBase, distance units, external gear ratio 


void tuned_constants()
{
//  Chassis.set_drive_constants(12, 0.35, 0.08, 7,0);//last tune
  Chassis.set_drive_constants(12, 0.35, 0.08, 7,0); // drive train (forward/backwards) tune
  Chassis.set_heading_constants(6, .4, 0, 1, 0); // turning tune
  Chassis.set_turn_constants(12, 0.20, 0.005, 1.25, 15);
  Chassis.set_swing_constants(12, .3, .001, 2, 15);
  Chassis.set_drive_exit_conditions(1.5, 300, 1500);
  Chassis.set_turn_exit_conditions(1, 300, 1000);
  Chassis.set_swing_exit_conditions(1, 300, 1000);

}

//Drive Chassis(LeftDriveSmart,RightDriveSmart,InertialPort, 3.25, 0.6, 360);



// motor leftDriveA = motor(PORT9, ratio6_1, true);  
// motor leftDriveB = motor(PORT10, ratio6_1, true);   
// motor leftDriveC = motor(PORT7, ratio6_1, true);   
// motor leftDriveD = motor(PORT8, ratio6_1, true);
// motor rightDriveA = motor(PORT3, ratio6_1, false);
// motor rightDriveB = motor(PORT4, ratio6_1, false);
// motor rightDriveC = motor(PORT1, ratio6_1, false);
// motor rightDriveD = motor(PORT2, ratio6_1, false);
// motor Catapult = motor(PORT11,ratio36_1,true);

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

// prints stuff to screen
void BrainPrint(){
  Brain.Screen.clearScreen();
  Brain.Screen.print("what the sigma");
  Brain.Screen.print(InertialPort);
  Brain.Screen.print(DriveTrainInertial.heading());
}

void pre_auton(void) {
BrainPrint();
Intake.setVelocity(100,percent);
tuned_constants();
Chassis.Gyro.calibrate();
  while (Chassis.Gyro.isCalibrating()) 
  {
    wait(25, msec);
  }
  Brain.Screen.clearScreen();


  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

void manualAutonomous(void){
  // tuning
  DriveTrain.setTurnVelocity(100,pct); // sets turn velocity of motor, will NOT run motor
  DriveTrain.setTurnConstant(0.3); // kp, sets sensitivity of turning
  DriveTrain.setDriveVelocity(100, pct); // sets drive move forward velocity

  DriveTrain.driveFor(forward, 40, inches); // tells to move 2 inches forward
  DriveTrain.turnFor(90, degrees);
  
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {

// Chassis.drive_distance(10);
// Chassis.turn_to_angle(90);
// Chassis.drive_distance(10);
// Chassis.turn_to_angle(180);
// Chassis.drive_distance(10);
// Chassis.turn_to_angle(270);
// Chassis.drive_distance(10);
// Chassis.turn_to_angle(360);
// Chassis.drive_distance(10);


#if MANUAL
  manualAutonomous();
#else
  #if Alliance
    #if Closeside
    CloseSideAuton();
    #else
    FarsideAuton();
    #endif
  #else
    #if Closeside
    CloseSideAutonBlue();
    #else
    FarsideAutonBlue();
    #endif
  #endif
#endif
  
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop

  // declare booleans here to reset when user control program starts
  bool IsShooting = false;

  while (1) {

    // Axis 3 Controls Left Drive
    if(abs(Controller1.Axis3.position()) > 30)
      LeftDriveSmart.spin(forward, Controller1.Axis3.position(), pct);

    else
      LeftDriveSmart.stop(coast);

    // Axis 2 Controls Right Drive
    if(abs(Controller1.Axis2.position())> 30) 
    RightDriveSmart.spin(forward,Controller1.Axis2.position(),pct);

    else
     RightDriveSmart.stop(coast);


    // Intake Button R1 spins it forward (takes ball in)
    if(Controller1.ButtonR1.pressing())
    {
      Intake.spin(forward,100,pct);
    }

    // Intake button R2 spins it backwards (spits ball out)
    else if(Controller1.ButtonR2.pressing())
    {
      Intake.spin(reverse,100,pct);
    }

    else
    Intake.stop(hold);


    // if IsShooting bool is true spin catapult forward, else stop
    if(IsShooting)
    Catapult.spin(fwd,100,pct);
    else
    Catapult.stop();

    // toggles the isShooting bool with X
    if(Controller1.ButtonX.pressing()){
      IsShooting = !IsShooting;
      waitUntil(!Controller1.ButtonX.pressing());
    }
    
    // toggles the Wings with L2
    if(Controller1.ButtonL2.pressing() && Hang.value() == false)
    {
      if(Wings.value())
      Wings.set(false);
      else
      Wings.set(true);
      waitUntil(!Controller1.ButtonL2.pressing()); 
    }

    // sets Hang to true by pressing A and Left together
    if(Controller1.ButtonA.pressing() && Controller1.ButtonLeft.pressing())
    {
      Hang.set(true);
    }

    // sets Hang to false by pressing L2
    if(Controller1.ButtonL2.pressing() && Hang.value() == true)
    {
      Hang.set(false);
      waitUntil(!Controller1.ButtonL2.pressing()); 
    }  

    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
