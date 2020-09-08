#ifndef InterfacePanel_H
#define InterfacePanel_H


#pragma once

#include "InterfaceObject.h"
#include <string>

using namespace std;

class InterfacePanel :
	public InterfaceObject
{
public:
	InterfacePanel(void);
	~InterfacePanel(void);
	void Interact(double, double, bool, bool);
};



#endif