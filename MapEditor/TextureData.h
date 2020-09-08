#ifndef TextureData_H
#define TextureData_H



#pragma once


#include <d3d9.h>
#include <d3dx9.h>


#include <vector>



class TextureData
{
public:
	int index;
	int height;
	int material;
	string name;
	std::vector<LPDIRECT3DTEXTURE9> terrainTexture;
	D3DMATERIAL9 terrainMaterial;

private:


protected:

};

#endif