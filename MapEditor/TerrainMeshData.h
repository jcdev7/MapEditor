#ifndef TerrainMeshData_H
#define TerrainMeshData_H

class MapList;      //forward declaration



#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "MapData.h"


#include "MapList.h"


#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library

class TerrainMeshData
{
public:
	TerrainMeshData(LPDIRECT3DDEVICE9);
	~TerrainMeshData();
	LPD3DXMESH GetTerrainMesh(MapList*, int, int, int, bool);
	LPD3DXMESH CombineMeshes();
	LPD3DXMESH GetTerrainBlendingMesh(MapList*, int, int, int, int);
	void Release();
private:
	float TerrainVertices2[100 * 100 * 6][3];
	LPDIRECT3DDEVICE9       Graphics_Device;
protected:
	LPD3DXMESH Temp_Terrain_Mesh;
	IDirect3DVertexBuffer9* NewTerrain_Buffer; // Buffer to hold vertices
};


#endif
