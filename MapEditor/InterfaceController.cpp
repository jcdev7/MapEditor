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
				//returnAction.nextScreen = 2;
				returnAction.nextScreen = InterfaceAction::Next_Screen::Screen_NewGame;
			}
		}
		if (buttonName == "Quit")
		{
			if (buttonPressed == true)
			{
				//returnAction.nextScreen = 3;
				returnAction.nextScreen = InterfaceAction::Next_Screen::Screen_Quit;
			}
		}
	}
	else if (interfaceName == "NewProfile")
	{
		if (buttonName == "BackToMain")
		{
			if (buttonPressed == true)
			{
				//returnAction.nextScreen = 1;
				returnAction.nextScreen = InterfaceAction::Next_Screen::Screen_MainMenu;
			}
		}
		if (buttonName == "Game")
		{
			if (buttonPressed == true)
			{
				//returnAction.nextScreen = 5;
				returnAction.nextScreen = InterfaceAction::Next_Screen::Screen_GameMenu;
			}
		}
	}
	else
	{
		//returnAction.nextScreen = 0;
		returnAction.nextScreen = InterfaceAction::Next_Screen::Screen_NoChange;
	}



	return returnAction;
}

void InterfaceController::ResetController()
{

	interfaceName = "NULL";
	buttonName = "NULL";
	buttonPressed = false;


}