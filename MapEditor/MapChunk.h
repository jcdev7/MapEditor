#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <string>
#include "MapData.h"


#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library

using namespace std;

class MapChunk
{
public:
	std::vector<MapData> TerrainMapData;
	int index;//the index of the square
	string filename;
	int numSquares;
	int distanceToCamera;
	D3DXVECTOR3 position;
	float radius;
	bool visible;//whether or not it is visible to the camera

private:

protected:

};
