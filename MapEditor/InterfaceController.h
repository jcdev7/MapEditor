#ifndef InterfaceController_H
#define InterfaceController_H

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <string>

#include "InterfaceAction.h"

using namespace std;

class InterfaceController
{
public:
	InterfaceController(void);
	~InterfaceController(void);
	void Init();
	void Update(string, string, int);//called from each InterfaceObject that has actions
	InterfaceAction CheckController();//called from Interface to check which objects have changed state
	void ResetController();//called from Interface to reset once an action has been read

	string interfaceName;
	string buttonName;
	bool buttonPressed;

};
#endif

//this class controls all button presses and changing between screens within the same interface
//we use the name of the interfaceobject to know which object is clicked
//within Interface, we check the controller to see what actions have happened from which buttons each frame