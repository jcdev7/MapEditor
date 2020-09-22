#include "InterfaceController.h"

InterfaceController::InterfaceController(void)
{
}

InterfaceController::~InterfaceController(void)
{
}

void InterfaceController::Init()
{

	interfaceName = "NULL";
	buttonName = "NULL";
	buttonPressed = false;


}

void InterfaceController::UpdateScreen(string interfaceN, string objectN, int actionIndex)
{

	interfaceName = interfaceN;
	buttonName = objectN;
	buttonPressed = true;


}

InterfaceAction InterfaceController::CheckController()
{
	InterfaceAction returnAction;

	returnAction.currentScreen = interfaceName;

	if (interfaceName == "MainMenu")
	{
		if (buttonName == "FreePlay")
		{
			if (buttonPressed == true)
			{
				returnAction.nextScreen = 2;
			}
		}
		if (buttonName == "Quit")
		{
			if (buttonPressed == true)
			{
				returnAction.nextScreen = 3;
			}
		}
	}
	else if (interfaceName == "NewProfile")
	{
		if (buttonName == "BackToMain")
		{
			if (buttonPressed == true)
			{
				returnAction.nextScreen = 1;
			}
		}
		if (buttonName == "Game")
		{
			if (buttonPressed == true)
			{
				returnAction.nextScreen = 5;
			}
		}
	}
	else
	{
		returnAction.nextScreen = 0;
	}



	return returnAction;
}

void InterfaceController::ResetController()
{

	interfaceName = "NULL";
	buttonName = "NULL";
	buttonPressed = false;


}