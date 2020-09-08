#include "InterfaceController.h"

InterfaceController::InterfaceController(void)
{
}

InterfaceController::~InterfaceController(void)
{
}

void InterfaceController::Init()
{

	this->interfaceName = "NULL";
	this->buttonName = "NULL";
	this->buttonPressed = false;


}

void InterfaceController::Update(string interfaceN, string objectN, int actionIndex)
{

	this->interfaceName = interfaceN;
	this->buttonName = objectN;
	this->buttonPressed = true;


}

InterfaceAction InterfaceController::CheckController()
{
	InterfaceAction returnAction;

	returnAction.currentScreen = this->interfaceName;

	if (this->interfaceName == "MainMenu")
	{
		if (this->buttonName == "FreePlay")
		{
			if (this->buttonPressed == true)
			{
				returnAction.nextScreen = 2;
			}
		}
		if (this->buttonName == "Quit")
		{
			if (this->buttonPressed == true)
			{
				returnAction.nextScreen = 3;
			}
		}
	}
	else if (this->interfaceName == "NewProfile")
	{
		if (this->buttonName == "BackToMain")
		{
			if (this->buttonPressed == true)
			{
				returnAction.nextScreen = 1;
			}
		}
		if (this->buttonName == "Game")
		{
			if (this->buttonPressed == true)
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

	this->interfaceName = "NULL";
	this->buttonName = "NULL";
	this->buttonPressed = false;


}