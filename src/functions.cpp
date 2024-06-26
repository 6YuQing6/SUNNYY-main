#include "functions.h"
#include <config.h>
using namespace std;
using namespace vex;


bool Auton = true;


void FarsideAuton()
{
  Intake.spin(vex::directionType::fwd);
  Chassis.drive_distance(60);
  Chassis.drive_distance(-10);
  Chassis.turn_to_angle(270);
  Intake.spin(vex::directionType::rev);
  Chassis.drive_distance(40); 
}

void CloseSideAuton()
{
  Intake.spin(vex::directionType::fwd);
  Chassis.drive_distance(60);
  Chassis.drive_distance(-10);
  Chassis.turn_to_angle(270);
  Intake.spin(vex::directionType::rev);
  Chassis.drive_distance(40); 
}

void FarsideAutonBlue()
{
  Intake.spin(vex::directionType::fwd);
  Chassis.drive_distance(60);
  Chassis.drive_distance(-10);
  Chassis.turn_to_angle(270);
  Intake.spin(vex::directionType::rev);
  Chassis.drive_distance(40); 
}

void CloseSideAutonBlue()
{
 TakeMidTriball();
}



void TakeMidTriball()
{
 Intake.spin(vex::directionType::fwd);
  Chassis.drive_distance(60);
  Chassis.drive_distance(-10);
  Chassis.turn_to_angle(270);
  Intake.spin(vex::directionType::rev);
  Chassis.drive_distance(40); 
}