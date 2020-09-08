#ifndef ObjectInstance_H
#define ObjectInstance_H

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library

class ObjectInstance
{
public:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	int heightMapX, heightMapZ;//the coordinates on the heightMap grid
	int heightMapY;//the height

	D3DXMATRIX scale_and_translate;



private:


protected:

};

#endif