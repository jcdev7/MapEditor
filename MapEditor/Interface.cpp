#pragma once

#include <vector>

#include "Interface.h"


using namespace std;


Interface::Interface(void)
{
}

Interface::~Interface(void)
{
}


void Interface::Init(string nC, int sW, int sH, int t, LPDIRECT3DDEVICE9 GraphicsDevice)
{
	this->interfaceController = new InterfaceController();
	this->interfaceController->Init();
	this->name = nC;
	screenWidth = sW;
	screenHeight = sH;
	type = t;
	Graphics_Device = GraphicsDevice;

	if (type == 0)
	{
		InitEditorInterface();
	}
	if (type == 1)
	{
		InitMainMenuInterface();
	}
	if (type == 2)
	{
		InitLoadingScreen();
	}



}

void Interface::InitEditorInterface()
{
	this->name = "Editor";

	//format of init:
	//offset, offset, size, size
	/*
	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("Editor", "background", Graphics_Device, screenWidth, screenHeight, 0, .8, 1, .2, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);
	*/

	return;

	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init(this->name, "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\BlankPanel.tga", NULL, interfaceController);
	




	InterfacePanel* menuPanelBackground = new InterfacePanel();
	menuPanelBackground->Init(this->name, "OptionsPanel", Graphics_Device, screenWidth, screenHeight, 0, .9, .8, .2, "MainMenu\\white.tga", menuBackground, interfaceController);

	//add buttons

	InterfaceButton* NewProfile = new InterfaceButton();
	NewProfile->Init(this->name, "NewProfile", Graphics_Device, screenWidth, screenHeight, .4, .15, .2, .1, "MainMenu\\NewProfileUp.jpg", menuPanelBackground, interfaceController);
	NewProfile->InitButton("MainMenu\\NewProfileDown.jpg");
	NewProfile->fullscreen = false;
	menuPanelBackground->interfaceObjects.push_back(NewProfile);



	//finished adding buttons

	menuBackground->interfaceObjects.push_back(menuPanelBackground);

	interfaceObjects.push_back(menuBackground);
}


void Interface::InitMainMenuInterface()
{

	this->name = "MainMenu";

	//format of init:
	//offset, offset, size, size

	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("MainMenu", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);




	InterfacePanel* menuPanelBackground = new InterfacePanel();
	menuPanelBackground->Init("MainMenu", "OptionsPanel", Graphics_Device, screenWidth, screenHeight, .3, .2, .4, .75, "MainMenu\\MarblePanel.jpg", menuBackground, interfaceController);

	//add buttons

	InterfaceButton* FreePlay = new InterfaceButton();
	FreePlay->lockHeightToWidthRatio = true;
	FreePlay->heightToWidthRatio = .75f;
	FreePlay->Init("MainMenu", "FreePlay", Graphics_Device, screenWidth, screenHeight, .4, .45, .2, .1, "MainMenu\\FreePlayUp.jpg", menuPanelBackground, interfaceController);
	FreePlay->InitButton("MainMenu\\FreePlayDown.jpg");
	FreePlay->fullscreen = false;
	menuPanelBackground->interfaceObjects.push_back(FreePlay);



	InterfaceButton* Quit = new InterfaceButton();
	Quit->Init("MainMenu", "Quit", Graphics_Device, screenWidth, screenHeight, .4, .75, .2, .1, "MainMenu\\QuitUp.jpg", menuPanelBackground, interfaceController);
	Quit->InitButton("MainMenu\\QuitDown.jpg");
	Quit->fullscreen = false;
	menuPanelBackground->interfaceObjects.push_back(Quit);


	//finished adding buttons

	interfaceObjects.push_back(menuPanelBackground);





}
void Interface::InitNewProfile()
{

	this->name = "NewProfile";


	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("NewProfile", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);

	InterfaceButton* BackToMain = new InterfaceButton();
	BackToMain->Init("NewProfile", "BackToMain", Graphics_Device, screenWidth, screenHeight, .025, .025, .1, .075, "MainMenu\\BackUp.jpg", menuBackground, interfaceController);
	BackToMain->InitButton("MainMenu\\BackDown.jpg");
	BackToMain->fullscreen = false;
	menuBackground->interfaceObjects.push_back(BackToMain);

	InterfacePanel* menuPanelBackground = new InterfacePanel();
	menuPanelBackground->Init("NewProfile", "DungeonPanel", Graphics_Device, screenWidth, screenHeight, .6, .1, .3, .8, "MainMenu\\MarblePanel.jpg", menuBackground, interfaceController);

	InterfaceButton* ToGame = new InterfaceButton();
	ToGame->Init("NewProfile", "Game", Graphics_Device, screenWidth, screenHeight, .3, .2, .4, .1, "MainMenu\\Dungeon1Up.jpg", menuPanelBackground, interfaceController);
	ToGame->InitButton("MainMenu\\Dungeon1Down.jpg");
	ToGame->fullscreen = false;
	menuPanelBackground->interfaceObjects.push_back(ToGame);

	menuBackground->interfaceObjects.push_back(menuPanelBackground);
}

void Interface::InitLoadProfile()
{
	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("LoadProfile", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);
}

void Interface::InitFreePlay()
{
	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("FreePlay", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);
}

void Interface::InitOptions()
{
	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("Options", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "MainMenu\\menuBackground2.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);
}



void Interface::InitLoadingScreen()
{

	this->name = "LoadingScreen";

	InterfacePanel* menuBackground = new InterfacePanel();
	menuBackground->Init("MainMenu", "background", Graphics_Device, screenWidth, screenHeight, 0, 0, 1, 1, "LoadingScreens\\testLoadingScreen1.jpg", NULL, interfaceController);
	interfaceObjects.push_back(menuBackground);

	this->interfaceController->UpdateScreen(this->name, "NULL", 0);

}

void Interface::Update(HWND hWnd, double mX, double mY, bool mL, bool mR)
{
	mouseX = mX;
	mouseY = mY;
	mouseLeft = mL;
	mouseRight = mR;


	//resize interface if screen dimensions have changed

	float newScreenWidth = screenWidth;
	float newScreenHeight = screenHeight;
	if (IsWindow(hWnd))
	{
		DWORD windowsStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD extendedWindowsStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU windowsMenu = GetMenu(hWnd);
		bool hasMenu = windowsMenu;

		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		AdjustWindowRectEx(&windowRect, windowsStyle, hasMenu, extendedWindowsStyle);

		newScreenWidth = windowRect.right - windowRect.left;
		newScreenHeight = windowRect.bottom - windowRect.top;

	}
	if (newScreenWidth != screenWidth || newScreenHeight != screenHeight)
	{
		screenWidth = newScreenWidth;
		screenHeight = newScreenHeight;
		for (int i = 0; i < interfaceObjects.size(); i++)
		{
			interfaceObjects[i]->Resize(newScreenWidth, newScreenHeight);
		}
	}


	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		interfaceObjects[i]->Update(mouseX, mouseY, mouseLeft, mouseRight);
	}

	if (mouseLeft == true)
	{
		//ChangeScreen(2);
	}

	InterfaceAction checkC;
	checkC = this->interfaceController->CheckController();
	if (checkC.nextScreen == 1)
	{
		this->interfaceController->ResetController();
		ChangeScreen(1);
	}
	if (checkC.nextScreen == 2)
	{
		this->interfaceController->ResetController();
		ChangeScreen(2);
	}
	if (checkC.nextScreen == 3)
	{
		this->interfaceController->ResetController();
		PostQuitMessage(0);
	}
	if (checkC.nextScreen == 5)
	{
		this->interfaceController->ResetController();
		ChangeScreen(5);
	}


}

void Interface::ChangeScreen(int t)
{

	//delete the old screen
	this->DeleteInterface();


	//set the new screen
	this->type = t;

	//init the new screen
	if (type == 0)
	{
		InitEditorInterface();
	}
	if (type == 1)//main menu
	{
		InitMainMenuInterface();
	}
	if (type == 2)//new profile
	{
		InitNewProfile();
		//InitLoadingScreen();
	}
	if (type == 3)//load profile
	{
		InitLoadingScreen();
	}
	if (type == 4)//free play
	{
		InitLoadingScreen();
	}
	if (type == 5)//options
	{
		InitLoadingScreen();
	}
	if (type == 6)//quit
	{
		InitLoadingScreen();
	}


}

void Interface::Draw()
{

	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		if (interfaceObjects[i]->visible)
		{
			interfaceObjects[i]->Draw();
		}
	}


}

void Interface::DeleteInterface()
{
	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		//delete all sub objects
		interfaceObjects[i]->DeleteObject();
	}

	int tempSize = interfaceObjects.size();
	for (int i = 0; i < tempSize; i++)
	{
		delete interfaceObjects.back();
		interfaceObjects.pop_back();
	}

	interfaceObjects.clear();

}