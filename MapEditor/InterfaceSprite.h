#ifndef InterfaceSprite_H
#define InterfaceSprite_H


#pragma once


#include <string>

using namespace std;

//this class is returned by the InterfaceController
class InterfaceSprite
{
public:

	bool visible;
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 texture;
	int textureWidth;
	int textureHeight;

};

#endif
