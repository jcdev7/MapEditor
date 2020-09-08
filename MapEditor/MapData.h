#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <string>

#include "MapEdgeData.h"
#include "ObjectData.h"

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library


using namespace std;

class MapData
{
public:
	MapData(void);
	~MapData(void);

	int squareNum;//the index of the square
	string filename;
	int width, length;
	int highPoint, lowPoint;//the highest and lowest points in the heightmap (for calculating bounding boxes)
	int distanceToCamera;
	bool visible;//whether or not it is visible to the camera
	int detailLevel, oldDetailLevel;//the level of detail (based on distance from the camera)
	bool detailLevelUpToDate;//whether the mesh has actually been created with the correct detailLevel

	bool beingUpdated;//tells whether the square is being updated in a thread

					  //these tell whether an edge has been matched to a lower detail square in the given direction
					  //note: if I ever add in the ability to have multiple smaller squares bordering one big one,
					  //then I'll have to change these bools to vectors of objects containing bools and other edge data
	bool NMatched;
	bool SMatched;
	bool EMatched;
	bool WMatched;

	D3DXVECTOR3 position;
	std::vector<MapEdgeData> edges;

	float heightMap[101][101];
	int depthMap[100][100];
	int materialMap[100][100];
	int textureMap[100][100];

	std::vector<int> currentTextures;//the textures used in this square

	ID3DXMesh *Terrain_Mesh;//optimized mesh for drawing
	ID3DXMesh *Target_Terrain_Mesh;//unoptimized mesh for picking
	ID3DXMesh *Terrain_Blending_Mesh[8];//optimized meshes for drawing the blended edges


										//the following are temporary meshes for multithreaded updating
	ID3DXMesh *temp_Terrain_Mesh;//optimized mesh for drawing
	ID3DXMesh *temp_Target_Terrain_Mesh;//unoptimized mesh for picking
	ID3DXMesh *temp_Terrain_Blending_Mesh[8];//optimized meshes for drawing the blended edges


											 //the two parts of the bounding box
	D3DXVECTOR3 BBMin;
	D3DXVECTOR3 BBMax;


	//SceneryObject data:
	std::vector<ObjectData> objectMeshes;
	int numObjects;


private:

protected:

};
