#ifndef InterfaceButton_H
#define InterfaceButton_H

#pragma once

#include <string>

#include "InterfaceObject.h"

class InterfaceButton :
	public InterfaceObject
{
public:
	InterfaceButton(void);
	~InterfaceButton(void);
	void InitButton(string);
	void Interact(double, double, bool, bool);
	bool pressed;
};
#endif