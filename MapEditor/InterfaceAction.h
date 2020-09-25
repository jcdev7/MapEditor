#ifndef InterfaceAction_H
#define InterfaceAction_H


#pragma once


#include <string>

using namespace std;

//this class is returned by the InterfaceController
class InterfaceAction
{
public:


	enum Next_Screen { Screen_MainMenu, Screen_NewGame, Screen_LoadingScreen, Screen_GameMenu, Screen_Quit, Screen_NoChange};

	string objectType;
	//int nextScreen;
	Next_Screen nextScreen;
	string currentScreen;


};

#endif
