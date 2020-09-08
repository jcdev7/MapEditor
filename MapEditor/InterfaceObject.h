#ifndef InterfaceObject_H
#define InterfaceObject_H


//class Interface;

//class InterfaceController;

#pragma once

//#include "Interface.h"

#include "InterfaceController.h"
#include "InterfaceAction.h"
#include "InterfaceSprite.h"

#include <d3d9.h>
#include <d3dx9.h>



#include <vector>
#include <string>



using namespace std;

class InterfaceObject
{
public:
	InterfaceObject(void);
	~InterfaceObject(void);

	//Interface *tempinterface;

	void Init(string interfaceName, string name, LPDIRECT3DDEVICE9, int, int, double, double, double, double, string, InterfaceObject *currentParent, InterfaceController *currentController);//passes the screen width, height, and an int for the type of interface
																																															  //abstract functions, only the subclass versions will ever be called
																																															  //virtual void Update(double, double) = 0;//update passes mouse information
																																															  //virtual void Draw() = 0;
	void Update(double, double, bool, bool);//update passes mouse information
	void Draw();
	void SetPosition(D3DXVECTOR3);
	void SetScale(D3DXVECTOR3);
	void SetScreenHeight(int);
	void SetScreenWidth(int);

	void DeleteObject();

	virtual void Interact(double, double, bool, bool);//a type specific function, for example is called when pressed if a button

	int GetObjectHeight();
	int GetObjectWidth();

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetScale();

	void SetOffset(D3DXVECTOR2);
	void SetDimensions(D3DXVECTOR2);

	//these functions will iterate through all parents, getting the combined offset for the contained object
	D3DXVECTOR2 GetParentPixelOffset();
	D3DXVECTOR2 GetParentPixelDimensions();

	D3DXVECTOR2 GetParentPercentDimensions();

	D3DXVECTOR2 GetRootScreenSize();

	InterfaceObject *parent;//set in Interface.cpp after Init

	InterfaceController *controller;

	string interfaceName;
	string name;

	bool visible;

	bool movable;

	bool fullscreen;

	D3DXVECTOR3 initialOffset;

	//screen size in pixels
	float screenWidth;
	float screenHeight;

	//image size in pixels
	float baseWidth;
	float baseHeight;

	D3DXVECTOR3 backgroundPosition;
	D3DXVECTOR3 backgroundScale;

	//these are used if the object is in another object
	bool hasParent;
	D3DXVECTOR3 parentBackgroundPosition;
	D3DXVECTOR3 parentBackgroundScale;



	//new variables for the offset/size within the parent's screen size (pixels)
	D3DXVECTOR2 pixelOffset;
	D3DXVECTOR2 pixelDimensions;

	//new variables for the offset/size within the parent's screen size (percentage)
	D3DXVECTOR2 percentOffset;
	D3DXVECTOR2 percentDimensions;









	D3DXMATRIX mScreenScale;

	D3DXMATRIX mScale;
	D3DXMATRIX mTranslation;
	D3DXMATRIX mScaleAndTranslate;

	LPDIRECT3DTEXTURE9 backgroundTexture;
	LPD3DXSPRITE backgroundSprite;

	std::vector<InterfaceSprite*> interfaceSprites;


	LPD3DXBUFFER backgroundBuffer;

	D3DXVECTOR2 upperLeft;
	D3DXVECTOR2 lowerRight;

	//an object can contain other interface objects, most likely this list will be empty though
	std::vector<InterfaceObject*> interfaceObjects;


	LPDIRECT3DDEVICE9 Graphics_Device;





};


#endif