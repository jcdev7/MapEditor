#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library

class MapEdgeData
{
public:
	char direction;
	int squareNum;//the index of the connecting square
	int chunkNum;//the index of the chunk containing the connecting square
	int leftIndex;
	int rightIndex;
private:

protected:

};
