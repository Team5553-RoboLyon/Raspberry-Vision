/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

void Robot::RobotInit()
{
	  //auto inst = nt::NetworkTableInstance::GetDefault();
	  table = NetworkTable::GetTable("datatable");
	  //entry = table->GetEntry("Value");
}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic()
{
	  //if (entry.Exists())
	  //{
	    std::string value = table->GetString("Value", "not found");

	    if(value == "Hello RoboRIO !!!")
	    {
	      std::cout << "Le message \"Hello RoboRIO !!!\" a été reçu" << std::endl << std::endl;
	      std::cout << "Envoi du message \"Hello RaspberryPi !!!\" ..." << std::endl << std::endl;
	      table->PutString("Value", "Hello RaspberryPi !!!");
	    }
	    else if(value == "not found")
	    {
	      std::cout << value << std::endl << std::endl;
	      std::cout << "Envoi du message \"Hello RaspberryPi !!!\" ..." << std::endl << std::endl;
	      table->PutString("Value", "Hello RaspberryPi !!!");
	    }
	    else
	    {
	      std::cout << value << std::endl << std::endl;
	    }
	  //}
}

void Robot::TestPeriodic() {}

START_ROBOT_CLASS(Robot)
