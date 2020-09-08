#ifndef MapList_H
#define MapList_H

class TerrainMeshData;      //forward declaration


#pragma once

#include <vector>

							//these  lines for file writing/reading
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <strsafe.h>

#include "MapChunk.h"
#include "MapData.h"
#include "MapEdgeData.h"

#include "ConnectingSquares.h"
#include "ConnectingSquare.h"
#include "EditedSquare.h"

#include "ConnectingEdge.h"

#include "TerrainMeshData.h"

#include "ObjectModel.h"
#include "ObjectInstance.h"

#include "TextureData.h"

#include "TerrainSubsetInfo.h"




class MapList
{
public:
	//std::vector<MapData> TerrainMapData;


	int numChunks;//the number of chunks in the map

	std::vector<MapChunk> TerrainMapChunks;

	std::vector<EditedSquare>EditedSquares;


	std::vector<TextureData> TerrainTextureData;

	std::vector<TerrainSubsetInfo> Subsets;


	//this list represents all models that exist in the game world
	//to improve efficiency, the list will be initialized with all members except the index as NULL
	//meshes and textures will be loaded and deleted as different chunks become active/inactive
	std::vector<ObjectModel> ModelList;

	ConnectingSquare FindConnectingSquare(ConnectingSquares&, int, int, int, int, int, int);

	ConnectingSquares FindConnectingTextureSquares(int, int, int, int, int, int);
	ConnectingSquares FindConnectingSquares(int, int, int, int, int, int);


	VOID ConnectEdges(int, int, int, int, int);

	//these functions should all stay here because they should be at the highest level of abstraction (e.g. only one object per world)
	//we can simply pass another parameter for the chunk index to them
	int NorthSquare(int, int, int, int, std::vector<int>&);
	int SouthSquare(int, int, int, int, std::vector<int>&);
	int EastSquare(int, int, int, int, std::vector<int>&);
	int WestSquare(int, int, int, int, std::vector<int>&);

	int NorthChunk(int, int, int, int, std::vector<int>&);
	int SouthChunk(int, int, int, int, std::vector<int>&);
	int EastChunk(int, int, int, int, std::vector<int>&);
	int WestChunk(int, int, int, int, std::vector<int>&);

	//quick versions
	int NorthSquareQuick(int, int, int, int);
	int SouthSquareQuick(int, int, int, int);
	int EastSquareQuick(int, int, int, int);
	int WestSquareQuick(int, int, int, int);

	int NorthChunkQuick(int, int, int, int);
	int SouthChunkQuick(int, int, int, int);
	int EastChunkQuick(int, int, int, int);
	int WestChunkQuick(int, int, int, int);

	void EditHeightMap(int, int, int, int, float, float, float, int, int, int, int, LPDIRECT3DDEVICE9);
	VOID UpdateTerrain(int, int, bool, LPDIRECT3DDEVICE9);

	VOID LoadWorldMap(string);//the map containing the chunk data

	VOID InitStaticObjectList(string, LPDIRECT3DDEVICE9);
	VOID InitTerrainTextureList(string, LPDIRECT3DDEVICE9);
	D3DMATERIAL9 SetupMaterial(int, LPDIRECT3DDEVICE9);

	VOID LoadMapChunk(int, string);

	VOID ReadSingleHeightMap(int, int, string);
	VOID ReadSingleTextureMap(int, int, string);
	VOID ReadSingleStaticObjectData(int, int, string);


	int FindModelIndex(int);

	VOID NewChunk(int);
	VOID NewSquare(int, int, int, int, int, LPDIRECT3DDEVICE9);
	VOID ConnectSquares(int, int, int, int);
	VOID NewMap();
	VOID SaveMap(string);

	//these two ints are for the edge connecting tools
	int activeSquare;
	int activeChunk;

private:
	LPD3DXMESH newMesh;

#define D3DFVF_IMPROVEDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

protected:

};


#endif