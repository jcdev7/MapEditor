#ifndef Interface_H
#define Interface_H

class InterfaceController;

//class InterfaceObject;



#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <string>


#include "InterfaceController.h"
#include "InterfaceAction.h"

#include "InterfaceObject.h"
//note: order of these includes is important, subclasses must be included after the parents
#include "InterfacePanel.h"
#include "InterfaceButton.h"



class Interface
{
public:
	Interface(void);
	~Interface(void);

	void Init(string nC, int sW, int sH, InterfaceAction::Next_Screen t, LPDIRECT3DDEVICE9 GraphicsDevice);//passes the screen width, height, and an int for the type of interface
	void Update(HWND windowHandle, double, double, bool, bool);//update passes mouse information
	void Draw();
	void InitEditorInterface();
	void InitMainMenuInterface();
	void InitNewProfile();
	void InitLoadProfile();
	void InitFreePlay();
	void InitOptions();
	void InitLoadingScreen();

	void ChangeScreen(InterfaceAction::Next_Screen);

	void DeleteInterface();


	std::vector<InterfaceObject> interfaceObjects2;

	InterfaceController *interfaceController;

	string name;

private:

	LPDIRECT3DDEVICE9 Graphics_Device;

	//screen size in pixels
	int screenWidth;
	int screenHeight;

	//initial screen size in pixels (needed when resizing screen)
	float initialScreenWidth;
	float initialScreenHeight;

	int type;

	double mouseX;
	double mouseY;

	bool mouseLeft;
	bool mouseRight;


	std::vector<InterfaceObject*> interfaceObjects;

};

#endif