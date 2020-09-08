#ifndef InterfaceAction_H
#define InterfaceAction_H


#pragma once


#include <string>

using namespace std;

//this class is returned by the InterfaceController
class InterfaceAction
{
public:

	string objectType;
	int nextScreen;
	string currentScreen;

};

#endif
