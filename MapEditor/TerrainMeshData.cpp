#include "TerrainMeshData.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "MapData.h"
#include "MapList.h"

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library

#define D3DFVF_IMPROVEDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// A structure for our custom vertex type. We added texture coordinates
struct IMPROVEDVERTEX
{
	D3DXVECTOR3 position; // The position
	D3DXVECTOR3 normal;   // The surface normal for the vertex
	D3DCOLOR    color;    // The color

	FLOAT       tu, tv;   // The texture coordinates

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
	};

};




TerrainMeshData::TerrainMeshData(LPDIRECT3DDEVICE9 Graphics_Device2)
{
	Graphics_Device = Graphics_Device2; // Our rendering device
}

TerrainMeshData::~TerrainMeshData()
{
}



//-----------------------------------------------------------------------------
// Name: GetTerrainMesh()
// Desc: creates the actual terrain mesh at the target chunk and map index
//-----------------------------------------------------------------------------
LPD3DXMESH TerrainMeshData::GetTerrainMesh(MapList *ML, int chunkIndex, int mapIndex, int DetailLevel, bool Opt)
{

	if (DetailLevel <= 0)//fix divide by zero error
	{
		DetailLevel = 1;
	}

	//lowest and highest points for creating the bounding box
	float lowestPoint = 0;
	float highestPoint = 0;

	int Mx = 0;
	int My = 1;


	int num = 0;
	int counter = 0;

	int MapHeight = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length;
	int MapWidth = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width;

	D3DXVECTOR3 worldPosition = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position;

	IMPROVEDVERTEX* TerrainVertices;

	D3DXVECTOR3 tempNormal_1;
	D3DXVECTOR3 tempNormal_2;

	D3DXVECTOR3 tempVertex_1;
	D3DXVECTOR3 tempVertex_2;
	D3DXVECTOR3 tempVertex_3;

	num = 0;

	//setup currentTextures list
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].currentTextures.clear();
	for (int tX = 0; tX < ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; tX++)
	{
		for (int tY = 0; tY < ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; tY++)
		{
			//check if texture is in currentTextures
			bool tempHasTexture = false;
			for (int tI = 0; tI < ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].currentTextures.size(); tI++)
			{
				if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].currentTextures[tI] == ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[tX][tY])
				{
					tempHasTexture = true;
					tI = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].currentTextures.size();
				}
			}
			if (tempHasTexture == false)
			{
				ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].currentTextures.push_back(ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[tX][tY]);
			}
		}
	}

	// Create the vertex buffer.
	if (FAILED(Graphics_Device->CreateVertexBuffer((MapWidth*MapHeight * 6 + ((MapWidth * 2 + MapHeight * 2) * 6)) * sizeof(IMPROVEDVERTEX),
		0, D3DFVF_IMPROVEDVERTEX,
		D3DPOOL_DEFAULT, &NewTerrain_Buffer, NULL)))
	{
		return NULL;
	}

	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	if (FAILED(NewTerrain_Buffer->Lock(0, 0, (void**)&TerrainVertices, 0)))
	{
		return NULL;
	}

	int num1 = 0;
	int num2 = 0;

	float tempHeight = 1.0;
	float tempHeight2 = 0.0;

	float tempY1 = 0;
	float tempY2 = 0;
	float tempY3 = 0;
	float tempY4 = 0;

	int edgeX = 0;
	int edgeY = 0;

	//these represent the indexes for the 4 edges of the square
	int edgeXLow = 0;
	int edgeXHigh = 0;
	int edgeYLow = 0;
	int edgeYHigh = 0;

	int random_h;


	int MeshWidth = MapWidth / DetailLevel;
	int MeshHeight = MapHeight / DetailLevel;

	float MWF = 0;
	float MHF = 0;

	MWF = MapWidth;
	MWF /= DetailLevel;

	MHF = MapHeight;
	MHF /= DetailLevel;


	num1 = 0;

	bool oddSquareX = true;
	bool oddSquareZ = true;
	float oddSF = 0;
	int oddSI = 0;

	int edgeDetailLevel = DetailLevel;
	int oldDetailLevel = DetailLevel;

	int edgeDetailLevelN = DetailLevel;
	int edgeDetailLevelE = DetailLevel;
	int edgeDetailLevelS = DetailLevel;
	int edgeDetailLevelW = DetailLevel;

	int oldDetailLevelN = DetailLevel;
	int oldDetailLevelE = DetailLevel;
	int oldDetailLevelS = DetailLevel;
	int oldDetailLevelW = DetailLevel;

	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = false;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].SMatched = false;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].EMatched = false;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].WMatched = false;


	//add if starements within this loop for highres-lowres edges
	for (int tNum = 0; tNum<(MeshWidth*MeshHeight * 6); tNum += (6))// num+=4 )
	{

		float mapX, mapZ;
		mapX = 0.0f;
		mapZ = 0.0f;

		mapX = worldPosition.x * 2;
		mapZ = worldPosition.z * 2;

		float squareSize = DetailLevel;

		float offsetX = (MWF / MeshWidth);
		float offsetZ = (MHF / MeshHeight);

		if (tNum % (MeshWidth * 6) == 0 && tNum != 0)//reach end of row
		{
			num2 += (DetailLevel);

			num1 = 0;
			tempHeight = 0;
			tempHeight2 = 0;
		}

		//add if statements to determine which edge edgeSideHL should be
		int edgeSideHL = 0;

		//x,y,z    y is up/down

		squareSize = 2 * DetailLevel*offsetX;

		edgeXHigh = num1 + DetailLevel;
		edgeXLow = num1;
		edgeYHigh = num2 + DetailLevel;
		edgeYLow = num2;

		std::vector<int> tempSquareList;

		bool NorthSide = false;
		bool EastSide = false;
		bool SouthSide = false;
		bool WestSide = false;

		if (tNum == (MeshWidth*MeshHeight * 6) - 6)
		{
			int dskjhfksdjfh = 0;
		}

		//add to if statements to make tempYs for the highres edge, if on an edge
		if (num2 == MapHeight - DetailLevel)//2*DetailLevel*(MeshHeight-1))
		{
			edgeYHigh = MapHeight;
			if (ML->NorthSquare(mapIndex, chunkIndex, num1, num2 + DetailLevel, tempSquareList) != 0)
			{
				edgeSideHL = 1;
				NorthSide = true;
			}
			else
			{
				edgeYHigh -= DetailLevel;
			}
			tempSquareList.clear();
		}
		if (num2 == 0)
		{
			edgeYLow = 0;
			edgeSideHL = 3;
			SouthSide = true;
		}
		if (num1 == MapWidth - DetailLevel)//4*DetailLevel*(MeshWidth-1))
		{
			edgeXHigh = MapWidth;
			if (ML->EastSquare(mapIndex, chunkIndex, num1 + DetailLevel, num2, tempSquareList) != 0)
			{
				edgeSideHL = 2;
				EastSide = true;
			}
			else
			{
				edgeXHigh -= DetailLevel;
			}

		}
		if (num1 == 0)
		{
			edgeXLow = 0;
			edgeSideHL = 4;
			WestSide = true;

		}


		///////////////////////////////----------------------------------------------

		float tempY1Low = 0;
		float tempY2Low = 0;
		float tempY3Low = 0;
		float tempY4Low = 0;

		float tempY1High = 0;
		float tempY2High = 0;
		float tempY3High = 0;
		float tempY4High = 0;


		tempY1Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYLow];
		tempY2Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY3Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];
		tempY4Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYHigh];

		tempY1High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYLow];
		tempY2High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY3High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];
		tempY4High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYHigh];



		int edgeSquare = mapIndex;
		int edgeChunk = chunkIndex;


		std::vector<int> chunkList;
		std::vector<int> squareList;

		oldDetailLevel = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].detailLevel;

		oldDetailLevelN = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].detailLevel;
		oldDetailLevelE = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].detailLevel;
		oldDetailLevelS = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].detailLevel;
		oldDetailLevelW = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].detailLevel;

		//for these edges, the x and y values correspond to the opposite of the direction the edge is in
		//(e.g. for the north edge, get the south x,y values of the square to the north)
		if (NorthSide)//(edgeSideHL == 1)//north
		{
			ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
			edgeSquare = ML->NorthSquare(mapIndex, chunkIndex, num1, num2 + DetailLevel, squareList);
			edgeChunk = ML->NorthChunk(mapIndex, chunkIndex, num1, num2 + DetailLevel, chunkList);
			edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			edgeDetailLevelN = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			if (edgeDetailLevelN < 1)//this is mostly for uninitialized values (but I don't think its necessary)
			{
				edgeDetailLevelN = DetailLevel;
			}
			else
			{
				if (pow(2.0, edgeDetailLevelN - 1) > DetailLevel)
				{
					oddSF = (float)(num1 / DetailLevel) / 2.0f;
					oddSI = (int)(num1 / DetailLevel) / 2;

					if (oddSF == oddSI)
					{
						oddSquareZ = false;
					}
					else
					{
						oddSquareZ = true;
					}

					if (oddSquareZ && edgeXLow - (edgeDetailLevelN / 2) >= 0)
					{
						tempY4Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 - DetailLevel][edgeYHigh];//low
						tempY4High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][edgeYHigh];
					}
					else if (!oddSquareZ)
					{
						tempY3Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1][edgeYHigh];//low
						tempY3High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel + DetailLevel][edgeYHigh];
					}
				}
			}
		}

		chunkList.clear();
		squareList.clear();


		if (SouthSide)//(edgeSideHL == 1)//south
		{
			ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
			edgeSquare = ML->SouthSquare(mapIndex, chunkIndex, num1, num2 + DetailLevel, squareList);
			edgeChunk = ML->SouthChunk(mapIndex, chunkIndex, num1, num2 + DetailLevel, chunkList);
			edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			edgeDetailLevelS = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			if (edgeDetailLevelS < 1)//this is mostly for uninitialized values (but I don't think its necessary)
			{
				edgeDetailLevelS = DetailLevel;
			}
			else
			{
				if (pow(2.0, edgeDetailLevelS - 1) > DetailLevel)
				{
					oddSF = (float)(num1 / DetailLevel) / 2.0f;
					oddSI = (int)(num1 / DetailLevel) / 2;

					if (oddSF == oddSI)
					{
						oddSquareZ = false;
					}
					else
					{
						oddSquareZ = true;
					}

					if (oddSquareZ && edgeXLow - (edgeDetailLevelS / 2) >= 0)
					{
						tempY1Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 - DetailLevel][0];//low
						tempY1High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][0];
					}
					else if (!oddSquareZ)
					{
						tempY2Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1][0];//low
						tempY2High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel + DetailLevel][0];
					}
				}
			}
		}

		chunkList.clear();
		squareList.clear();

		if (EastSide)//(edgeSideHL == 1)//east
		{
			ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
			edgeSquare = ML->EastSquare(mapIndex, chunkIndex, num1 + DetailLevel, num2, squareList);
			edgeChunk = ML->EastChunk(mapIndex, chunkIndex, num1 + DetailLevel, num2, chunkList);
			edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			edgeDetailLevelE = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			if (edgeDetailLevelE < 1)//this is mostly for uninitialized values (but I don't think its necessary)
			{
				edgeDetailLevelE = DetailLevel;
			}
			else
			{
				if (pow(2.0, edgeDetailLevelE - 1) > DetailLevel)
				{
					oddSF = (float)(num2 / DetailLevel) / 2.0f;
					oddSI = (int)(num2 / DetailLevel) / 2;

					if (oddSF == oddSI)
					{
						oddSquareX = false;
					}
					else
					{
						oddSquareX = true;
					}

					if (oddSquareX && edgeYLow - (edgeDetailLevelE / 2) >= 0)
					{
						tempY2Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][num2 - DetailLevel];//low
						tempY2High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][num2 + DetailLevel];
					}
					else if (!oddSquareX)
					{
						tempY3Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][num2];//low
						tempY3High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][num2 + DetailLevel + DetailLevel];
					}
				}
			}
		}

		chunkList.clear();
		squareList.clear();

		if (WestSide)//(edgeSideHL == 1)//west
		{
			ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
			edgeSquare = ML->WestSquare(mapIndex, chunkIndex, num1 + DetailLevel, num2, squareList);
			edgeChunk = ML->WestChunk(mapIndex, chunkIndex, num1 + DetailLevel, num2, chunkList);
			edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			edgeDetailLevelW = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;
			if (edgeDetailLevelW < 1)//this is mostly for uninitialized values (but I don't think its necessary)
			{
				edgeDetailLevelW = DetailLevel;
			}
			else
			{
				if (pow(2.0, edgeDetailLevelW - 1) > DetailLevel)
				{
					oddSF = (float)(num2 / DetailLevel) / 2.0f;
					oddSI = (int)(num2 / DetailLevel) / 2;

					if (oddSF == oddSI)
					{
						oddSquareX = false;
					}
					else
					{
						oddSquareX = true;
					}

					if (oddSquareX && edgeYLow - (edgeDetailLevelW / 2) >= 0)
					{
						tempY1Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[0][num2 - DetailLevel];//low
						tempY1High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[0][num2 + DetailLevel];
					}
					else if (!oddSquareX)
					{
						tempY4Low = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[0][num2];//low
						tempY4High = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[0][num2 + DetailLevel + DetailLevel];
					}
				}
			}
		}

		chunkList.clear();
		squareList.clear();

		edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;



		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYLow];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];
		tempY4 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYHigh];

		//Note: it is important that the highest and lowest point setting is done here before the edge corrections (since there are some errors there)
		if (tNum == 0)
		{
			lowestPoint = tempY1;
			highestPoint = tempY1;
		}

		if (tempY1 < lowestPoint)
		{
			lowestPoint = tempY1;
		}
		if (tempY2 < lowestPoint)
		{
			lowestPoint = tempY2;
		}
		if (tempY3 < lowestPoint)
		{
			lowestPoint = tempY3;
		}
		if (tempY4 < lowestPoint)
		{
			lowestPoint = tempY4;
		}
		if (tempY1 > highestPoint)
		{
			highestPoint = tempY1;
		}
		if (tempY2 > highestPoint)
		{
			highestPoint = tempY2;
		}
		if (tempY3 > highestPoint)
		{
			highestPoint = tempY3;
		}
		if (tempY4 > highestPoint)
		{
			highestPoint = tempY4;
		}

		bool needsUpdate = false;

		if ((edgeDetailLevelN > oldDetailLevelN || edgeDetailLevelS > oldDetailLevelS || edgeDetailLevelW > oldDetailLevelW || edgeDetailLevelE > oldDetailLevelE)
			&& (edgeDetailLevelN > 0 || edgeDetailLevelS > 0 || edgeDetailLevelW > 0 || edgeDetailLevelE > 0) && edgeSideHL != 0)
		{
			needsUpdate = true;
		}

		//THIS IS WHERE THE ERROR IS: when more than one edgeDetailLevel is used, only the last is remembered
		//TO FIX: add a new variable: highestEdgeDetailLevel or just add 4 N/S/W/E edgeDetailLevel variables
		//if(edgeDetailLevel > oldDetailLevel && edgeDetailLevel > 0 && edgeSideHL != 0)
		if (needsUpdate)
		{
			//if(edgeDetailLevel > oldDetailLevel && edgeDetailLevel > 0 && edgeSideHL != 0)
			//{


			if (oddSquareZ == true)
			{
				if (NorthSide)
				{
					tempY4 = tempY4Low;
					tempY4 += tempY4High;
					tempY4 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
				}
				if (SouthSide)
				{
					tempY1 = tempY1Low;
					tempY1 += tempY1High;
					tempY1 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].SMatched = true;
				}
			}
			if (oddSquareZ == false)
			{
				if (NorthSide)
				{
					tempY3 = tempY3Low;
					tempY3 += tempY3High;
					tempY3 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].NMatched = true;
				}
				if (SouthSide)
				{
					tempY2 = tempY2Low;
					tempY2 += tempY2High;
					tempY2 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].SMatched = true;
				}
			}
			///////////////////////////////////

			if (oddSquareX == true)
			{
				if (EastSide)
				{

					tempY2 = tempY2Low;
					tempY2 += tempY2High;
					tempY2 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].EMatched = true;

				}
				if (WestSide)
				{
					tempY1 = tempY1Low;
					tempY1 += tempY1High;
					tempY1 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].WMatched = true;
				}
			}
			if (oddSquareX == false)
			{
				if (EastSide)
				{
					tempY3 = tempY3Low;
					tempY3 += tempY3High;
					tempY3 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].EMatched = true;
				}
				if (WestSide)
				{
					tempY4 = tempY4Low;
					tempY4 += tempY4High;
					tempY4 /= 2.0f;
					ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].WMatched = true;
				}
			}


		}
		else
		{

		}
		///////////////////////////////----------------------------------------------

		/*
		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYLow];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];

		tempY4 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYHigh];
		*/
		//init low/high points


		//Prepare for 1st triangle of quad
		tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
		tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (0) + mapZ);
		tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);

		TerrainVertices[num + 2].position = tempVertex_1;
		TerrainVertices[num + 2].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);//.5
		TerrainVertices[num + 2].tu = 0;
		TerrainVertices[num + 2].tv = 0;

		TerrainVertices[num + 1].position = tempVertex_2;
		TerrainVertices[num + 1].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);//.5
		TerrainVertices[num + 1].tu = 1;
		TerrainVertices[num + 1].tv = 0;


		TerrainVertices[num + 0].position = tempVertex_3;
		TerrainVertices[num + 0].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
		TerrainVertices[num + 0].tu = 1;
		TerrainVertices[num + 0].tv = 1;

		//Prepare for 2nd triangle of quad
		tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY4, num2 * 2 * offsetZ + (squareSize)+mapZ);

		TerrainVertices[num + 4].position = tempVertex_3;

		TerrainVertices[num + 4].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
		TerrainVertices[num + 4].tu = 0;
		TerrainVertices[num + 4].tv = 1;

		num1 += (DetailLevel);



		TerrainVertices[num + 5].position = TerrainVertices[num + 0].position;

		TerrainVertices[num + 5].color = TerrainVertices[num + 0].color;
		TerrainVertices[num + 5].tu = TerrainVertices[num + 0].tu;
		TerrainVertices[num + 5].tv = TerrainVertices[num + 0].tv;

		TerrainVertices[num + 3].position = TerrainVertices[num + 2].position;

		TerrainVertices[num + 3].color = TerrainVertices[num + 2].color;
		TerrainVertices[num + 3].tu = TerrainVertices[num + 2].tu;
		TerrainVertices[num + 3].tv = TerrainVertices[num + 2].tv;



		//
		//Model of terrain square showing vertex values (6 needed)
		// 3            5
		//  ************* 2
		//  *          **
		//  *       *   *
		//  *    *      *
		//  * *         *
		//4 *************
		// 0             1
		//

		num += 6;

	}

	//TODO: add the outer edges in a seperate for loop
	int lastNum = (MeshWidth*MeshHeight * 6);
	//note: need to add another function to return the north/east/south/west x and z coordinates as well
	//but for now, we'll just assume that all connecting squares are the same size so we can use the old x and z
	int edgeSquare = 0;
	int edgeChunk = 0;
	D3DXVECTOR3 edgeVector;
	int edgeNum = 0;


	for (int edgeSide = 1; edgeSide <= 4; edgeSide++)
	{

		num1 = 0;
		num2 = 0;
		tempHeight = 1.0;
		tempHeight2 = 0.0;
		tempY1 = 0;
		tempY2 = 0;
		tempY3 = 0;
		edgeX = 0;
		edgeY = 0;
		//these represent the indexes for the 4 edges of the square
		edgeXLow = 0;
		edgeXHigh = 0;
		edgeYLow = 0;
		edgeYHigh = 0;

		edgeNum = lastNum;
		for (int eNum = lastNum; eNum < lastNum + (MeshWidth * 6); eNum += 6)
		{
			float mapX, mapZ;
			mapX = 0.0f;
			mapZ = 0.0f;

			mapX = worldPosition.x * 2;
			mapZ = worldPosition.z * 2;

			float squareSize = DetailLevel;

			float offsetX = (MWF / MeshWidth);
			float offsetZ = (MHF / MeshHeight);

			if ((edgeSide == 2 || edgeSide == 4) && eNum != lastNum)//reach end of row
			{
				num2 += (DetailLevel);

				num1 = 0;
				tempHeight = 0;
				tempHeight2 = 0;
			}

			//x,y,z    y is up/down


			squareSize = 2 * DetailLevel*offsetX;

			tempY1 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1][num2];
			tempY2 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1+DetailLevel][num2];
			tempY3 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1+DetailLevel][num2+DetailLevel];

			edgeXHigh = num1 + DetailLevel;
			edgeXLow = num1;
			edgeYHigh = num2 + DetailLevel;
			edgeYLow = num2;

			//add if statements to make tempYs for the highres edge, if on an edge
			if (num2 == 2 * DetailLevel*(MeshHeight - 1))
			{
				edgeYHigh = MapHeight;
			}
			if (num2 == 0)
			{
				edgeYLow = 0;
			}
			if (num1 == 4 * DetailLevel*(MeshWidth - 1))
			{
				edgeXHigh = MapWidth;

			}
			if (num1 == 0)
			{
				edgeXLow = 0;

			}

			//todo: set edge high and low according to which edge (use index and chunk from north/south/east/west functions) we are adding
			//if there is no neighboring square, simply set the heightmap to 0 and it'll be fine

			int edgeSquare = 0;
			int edgeChunk = 0;


			std::vector<int> chunkList;
			std::vector<int> squareList;

			//for these edges, the x and y values correspond to the opposite of the direction the edge is in
			//(e.g. for the north edge, get the south x,y values of the square to the north)
			if (edgeSide == 1)//north
			{
				edgeSquare = ML->NorthSquare(mapIndex, chunkIndex, num1, num2, squareList);
				edgeChunk = ML->NorthChunk(mapIndex, chunkIndex, num1, num2, chunkList);
				edgeYLow = 0;
				edgeYHigh = 0 + DetailLevel;
			}
			if (edgeSide == 2)//east
			{
				edgeSquare = ML->EastSquare(mapIndex, chunkIndex, num1, num2, squareList);
				edgeChunk = ML->EastChunk(mapIndex, chunkIndex, num1, num2, chunkList);
				edgeXLow = 0;
				edgeXHigh = 0 + DetailLevel;
			}
			if (edgeSide == 3)//south
			{
				edgeSquare = ML->SouthSquare(mapIndex, chunkIndex, num1, num2, squareList);
				edgeChunk = ML->SouthChunk(mapIndex, chunkIndex, num1, num2, chunkList);
				edgeYLow = MapHeight - DetailLevel;
				edgeYHigh = MapHeight;
			}
			if (edgeSide == 4)//west
			{
				edgeSquare = ML->WestSquare(mapIndex, chunkIndex, num1, num2, squareList);
				edgeChunk = ML->WestChunk(mapIndex, chunkIndex, num1, num2, chunkList);
				edgeXLow = MapWidth - DetailLevel;
				edgeXHigh = MapWidth;
			}

			edgeDetailLevel = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].detailLevel;


			tempY1 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYLow];
			tempY2 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYHigh];
			tempY3 = 0;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[edgeSquare].heightMap[edgeXLow][edgeYHigh];

			//todo: add a loop within this and the other tempY set area
			//the purpose is to find the average of all connecting squares if more than one exists
			//and to set the tempY values to that average
			if (edgeSquare != 0)
			{
				tempY1 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXLow][edgeYLow];
				tempY2 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYLow];
				tempY3 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYHigh];
			}
			//Prepare for 1st triangle of quad
			//tempVertex_1= D3DXVECTOR3(num1*2*offsetX+ (0) +mapX, tempY1, num2*2*offsetZ+ (0) +mapZ );
			//tempVertex_2= D3DXVECTOR3(num1*2*offsetX+ (squareSize) +mapX, tempY2, num2*2*offsetZ+ (0) +mapZ );
			//tempVertex_3= D3DXVECTOR3(num1*2*offsetX+ (squareSize) +mapX, tempY3, num2*2*offsetZ+ (squareSize) +mapZ );

			//new version (for edges)
			if (edgeSide == 1)//north
			{
				tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, squareSize*MeshHeight + mapZ);
				tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, squareSize*MeshHeight + mapZ);
				tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY3, squareSize*MeshHeight + squareSize + mapZ);
			}
			if (edgeSide == 2)//east
			{
				tempVertex_1 = D3DXVECTOR3(squareSize*MeshWidth + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_2 = D3DXVECTOR3(squareSize*MeshWidth + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_3 = D3DXVECTOR3(squareSize*MeshWidth + (squareSize)+mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);
			}
			if (edgeSide == 3)//south
			{
				tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, -squareSize + mapZ);
				tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, -squareSize + mapZ);
				tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY3, 0 + mapZ);
			}
			if (edgeSide == 4)//west
			{
				tempVertex_1 = D3DXVECTOR3(-squareSize + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_2 = D3DXVECTOR3(-squareSize + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_3 = D3DXVECTOR3(-squareSize + (squareSize)+mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);
			}

			TerrainVertices[num + 2].position = tempVertex_1;
			TerrainVertices[num + 2].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);//.5
			TerrainVertices[num + 2].tu = 0;
			TerrainVertices[num + 2].tv = 0;

			TerrainVertices[num + 1].position = tempVertex_2;
			TerrainVertices[num + 1].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);//.5
			TerrainVertices[num + 1].tu = 1;
			TerrainVertices[num + 1].tv = 0;


			TerrainVertices[num + 0].position = tempVertex_3;
			TerrainVertices[num + 0].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
			TerrainVertices[num + 0].tu = 1;
			TerrainVertices[num + 0].tv = 1;


			if (edgeSquare != 0)
			{
				tempY1 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYLow];
				tempY2 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXHigh][edgeYHigh];
				tempY3 = ML->TerrainMapChunks[edgeChunk].TerrainMapData[edgeSquare].heightMap[edgeXLow][edgeYHigh];
			}

			//Prepare for 2nd triangle of quad

			//new version (for edges)
			if (edgeSide == 1)//north
			{
				tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, squareSize*MeshHeight + mapZ);
				tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, squareSize*MeshHeight + squareSize + mapZ);
				tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY3, squareSize*MeshHeight + squareSize + mapZ);
			}
			if (edgeSide == 2)//east
			{
				tempVertex_1 = D3DXVECTOR3(squareSize*MeshWidth + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_2 = D3DXVECTOR3(squareSize*MeshWidth + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (squareSize)+mapZ);
				tempVertex_3 = D3DXVECTOR3(squareSize*MeshWidth + (0) + mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);
			}
			if (edgeSide == 3)//south
			{
				tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, -squareSize + mapZ);
				tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, 0 + mapZ);
				tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY3, 0 + mapZ);
			}
			if (edgeSide == 4)//west
			{
				tempVertex_1 = D3DXVECTOR3(-squareSize + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
				tempVertex_2 = D3DXVECTOR3(-squareSize + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (squareSize)+mapZ);
				tempVertex_3 = D3DXVECTOR3(-squareSize + (0) + mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);
			}

			TerrainVertices[num + 4].position = tempVertex_3;

			TerrainVertices[num + 4].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
			TerrainVertices[num + 4].tu = 0;
			TerrainVertices[num + 4].tv = 1;

			num1 += (DetailLevel);




			TerrainVertices[num + 5].position = TerrainVertices[num + 0].position;

			TerrainVertices[num + 5].color = TerrainVertices[num + 0].color;
			TerrainVertices[num + 5].tu = TerrainVertices[num + 0].tu;
			TerrainVertices[num + 5].tv = TerrainVertices[num + 0].tv;

			TerrainVertices[num + 3].position = TerrainVertices[num + 2].position;

			TerrainVertices[num + 3].color = TerrainVertices[num + 2].color;
			TerrainVertices[num + 3].tu = TerrainVertices[num + 2].tu;
			TerrainVertices[num + 3].tv = TerrainVertices[num + 2].tv;



			//
			//Model of terrain square showing vertex values (6 needed)
			// 3            5
			//  ************* 2
			//  *          **
			//  *       *   *
			//  *    *      *
			//  * *         *
			//4 *************
			// 0             1
			//

			num += 6;

		}
		lastNum = edgeNum;
	}//end of edgeSide loop

	NewTerrain_Buffer->Unlock();


	//
	// Create a mesh
	//

	HRESULT hr = 0;

	hr = D3DXCreateMeshFVF((MeshWidth*MeshHeight * 2 + ((MeshWidth * 2 + MeshHeight * 2) * 2)),
		(MeshWidth*MeshHeight * 6 + ((MeshWidth * 2 + MeshHeight * 2) * 6)),
		D3DXMESH_VB_MANAGED,
		IMPROVEDVERTEX::FVF_Flags, // This is how each vertex is laid out. 
		Graphics_Device,
		&Temp_Terrain_Mesh);


	IMPROVEDVERTEX* mVertices = NULL;




	Temp_Terrain_Mesh->LockVertexBuffer(0, (void**)&mVertices);
	{
		for (counter = 0; counter<(MeshWidth*MeshHeight * 6); counter++)
		{
			mVertices[counter] = TerrainVertices[counter];
		}
		if (Opt)
		{
			for (counter = (MeshWidth*MeshHeight * 6); counter<(MeshWidth*MeshHeight * 6 + ((MeshWidth * 2 + MeshHeight * 2) * 6)); counter++)
			{
				mVertices[counter] = TerrainVertices[counter];
			}
		}

	}
	Temp_Terrain_Mesh->UnlockVertexBuffer();



	WORD* pIndices = NULL;

	Temp_Terrain_Mesh->LockIndexBuffer(0, (void**)&pIndices);
	{
		for (counter = 0; counter<(MeshWidth*MeshHeight * 6); counter++)
		{
			pIndices[counter] = counter;
		}
		if (Opt)
		{
			for (counter = (MeshWidth*MeshHeight * 6); counter<(MeshWidth*MeshHeight * 6 + ((MeshWidth * 2 + MeshHeight * 2) * 6)); counter++)
			{
				pIndices[counter] = counter;
			}
		}

	}
	Temp_Terrain_Mesh->UnlockIndexBuffer();




	DWORD* pAttributes = NULL;
	Temp_Terrain_Mesh->LockAttributeBuffer(0, &pAttributes);
	{
		int texX, texY;

		for (counter = 0; counter<(MeshWidth*MeshHeight * 2); counter += 2)
		{
			texX = counter / 2;
			texY = 0;
			while (texX >= MeshWidth)
			{
				texX -= MeshWidth;
				texY += DetailLevel;
			}
			texX *= DetailLevel;


			if (texY >= MeshHeight*DetailLevel)
			{
				texY = MeshHeight - DetailLevel;
			}


			//note: change this implementation by storing a list of the textures in use and then setting up the subsets
			//based on the heights used for each texture
			//also, textures should only be open in memory if they are part of one of the active terrain texture lists
			//this will allow for an unliminited potential number of textures, with low memory usage

			//set up the subsets based on textures
			pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY];//evens are bottom right triangle
			pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY];//odds are top left triangle

																														//pAttributes[MeshWidth*MeshHeight*2-1] = 1;
		}
		if (Opt)
		{
			for (counter = MeshWidth*MeshHeight * 2; counter<(MeshWidth*MeshHeight * 2 + ((MeshWidth * 2 + MeshHeight * 2) * 2)); counter += 2)
			{
				pAttributes[counter] = 0;
				pAttributes[counter + 1] = 0;
			}
		}
	}

	Temp_Terrain_Mesh->UnlockAttributeBuffer();

	if (Opt)//if Optimization flag is set, do optimizations
	{
		//create adjacency buffer
		LPD3DXBUFFER pAdjacencyBuffer = NULL;
		D3DXCreateBuffer(Temp_Terrain_Mesh->GetNumFaces() * 3, &pAdjacencyBuffer);


		DWORD adjac[(16 + 2)*(16 + 2) * 6];
		Temp_Terrain_Mesh->GenerateAdjacency(0.1f, adjac);

		//New Idea: (to fix the inaccurate shading on the edges of meshes)
		//add another row on the outside of these meshes (take from neighboring meshes if they exist)
		//this outside row will not be included in the default subset, so it won't be displayed
		//it is only for more accurately calculating the normals for the outside edges
		//note: this will also fix the potential problem of the blending mesh not showing the blending at terrain square edges
		D3DXComputeNormals(Temp_Terrain_Mesh, adjac);

		// optimize mesh
		hr = Temp_Terrain_Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |    // Reorders faces to optimize for fewer attribute bundle state changes and enhanced ID3DXBaseMesh::DrawSubset performance.
			D3DXMESHOPT_COMPACT |     // Reorders faces to remove unused vertices and faces.
			D3DXMESHOPT_VERTEXCACHE // Reorders faces to increase the cache hit rate of vertex caches. // or D3DXMESHOPT_STRIPREORDER
			,  // Optimize the faces only; do not optimize the vertices.
			adjac,
			NULL, NULL, NULL);

		if (pAdjacencyBuffer != NULL)
			pAdjacencyBuffer->Release();


	}

	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.x = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position.x * 2;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.z = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position.z * 2;

	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.x = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.x + ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width * 2;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.z = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.z + ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length * 2;

	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.y = lowestPoint;
	ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.y = highestPoint;

	return Temp_Terrain_Mesh;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//one function for vertical and horizontal blending (another for diagonal)
LPD3DXMESH TerrainMeshData::GetTerrainBlendingMesh(MapList *ML, int chunkIndex, int mapIndex, int DetailLevel, int blendDirection)
{

	//4 alpha values
	int alpha1 = 1;
	int alpha2 = 1;
	int alpha3 = 1;
	int alpha4 = 1;

	//set up all the vertex alpha values
	if (blendDirection == 0)//north to south
	{
		alpha1 = 0;
		alpha2 = 0;
		alpha3 = 1;
		alpha4 = 1;
	}
	if (blendDirection == 1)//south to north
	{
		alpha1 = 1;
		alpha2 = 1;
		alpha3 = 0;
		alpha4 = 0;
	}
	if (blendDirection == 2)//east to west
	{
		alpha1 = 1;
		alpha2 = 0;
		alpha3 = 0;
		alpha4 = 1;
	}
	if (blendDirection == 3)//west to east
	{
		alpha1 = 0;
		alpha2 = 1;
		alpha3 = 1;
		alpha4 = 0;
	}

	if (blendDirection == 4)//north east
	{
		alpha1 = 0;
		alpha2 = 0;
		alpha3 = 1;
		alpha4 = 0;
	}
	if (blendDirection == 5)//south west
	{
		alpha1 = 1;
		alpha2 = 0;
		alpha3 = 0;
		alpha4 = 0;
	}

	if (blendDirection == 6)//north west
	{
		alpha1 = 0;
		alpha2 = 0;
		alpha3 = 0;
		alpha4 = 1;
	}
	if (blendDirection == 7)//south east
	{
		alpha1 = 0;
		alpha2 = 1;
		alpha3 = 0;
		alpha4 = 0;
	}

	int Mx = 0;
	int My = 1;


	int num = 0;
	int counter = 0;

	int MapHeight = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length;
	int MapWidth = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width;

	D3DXVECTOR3 worldPosition = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position;

	IMPROVEDVERTEX* TerrainVertices;

	D3DXVECTOR3 tempNormal_1;
	D3DXVECTOR3 tempNormal_2;

	D3DXVECTOR3 tempVertex_1;
	D3DXVECTOR3 tempVertex_2;
	D3DXVECTOR3 tempVertex_3;


	num = 0;



	// Create the vertex buffer.
	if (FAILED(Graphics_Device->CreateVertexBuffer((MapWidth*MapHeight * 6) * sizeof(IMPROVEDVERTEX),
		0, D3DFVF_IMPROVEDVERTEX,
		D3DPOOL_DEFAULT, &NewTerrain_Buffer, NULL)))
	{
		return NULL;
	}





	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	if (FAILED(NewTerrain_Buffer->Lock(0, 0, (void**)&TerrainVertices, 0)))
		return NULL;



	int num1 = 0;
	int num2 = 0;

	float tempHeight = 1.0;
	float tempHeight2 = 0.0;

	float tempY1 = 0;
	float tempY2 = 0;
	float tempY3 = 0;

	int edgeX = 0;
	int edgeY = 0;

	//these represent the indexes for the 4 edges of the square
	int edgeXLow = 0;
	int edgeXHigh = 0;
	int edgeYLow = 0;
	int edgeYHigh = 0;


	int random_h;

	int MeshWidth = MapWidth / DetailLevel;
	int MeshHeight = MapHeight / DetailLevel;

	float MWF = 0;
	float MHF = 0;

	MWF = MapWidth;
	MWF /= DetailLevel;

	MHF = MapHeight;
	MHF /= DetailLevel;


	num1 = 0;
	for (int tNum = 0; tNum<(MeshWidth*MeshHeight * 6); tNum += (6))// num+=4 )
	{

		float mapX, mapZ;
		mapX = 0.0f;
		mapZ = 0.0f;

		mapX = worldPosition.x * 2;
		mapZ = worldPosition.z * 2;

		float squareSize = DetailLevel;

		float offsetX = (MWF / MeshWidth);
		float offsetZ = (MHF / MeshHeight);

		if (tNum % (MeshWidth * 6) == 0 && tNum != 0)//reach end of row
		{
			num2 += (DetailLevel);

			num1 = 0;
			tempHeight = 0;
			tempHeight2 = 0;
		}

		//x,y,z    y is up/down

		squareSize = 2 * DetailLevel*offsetX;

		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1][num2];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][num2];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][num2 + DetailLevel];

		edgeXHigh = num1 + DetailLevel;
		edgeXLow = num1;
		edgeYHigh = num2 + DetailLevel;
		edgeYLow = num2;

		//add if statements to make tempYs for the highres edge, if on an edge
		if (num2 == 2 * DetailLevel*(MeshHeight - 1))
		{
			edgeYHigh = MapHeight;
		}
		if (num2 == 0)
		{
			edgeYLow = 0;
		}
		if (num1 == 4 * DetailLevel*(MeshWidth - 1))
		{
			edgeXHigh = MapWidth;

		}
		if (num1 == 0)
		{
			edgeXLow = 0;

		}


		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYLow];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];

		//Prepare for 1st triangle of quad
		tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
		tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (0) + mapZ);
		tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);


		TerrainVertices[num + 2].position = tempVertex_1;
		//D3DXVec3Cross(&TerrainVertices[num+0].normal,&tempNormal_1,&tempNormal_2);		
		TerrainVertices[num + 2].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, alpha1);//.5
		TerrainVertices[num + 2].tu = 0;
		TerrainVertices[num + 2].tv = 0;


		TerrainVertices[num + 1].position = tempVertex_2;
		//D3DXVec3Cross(&TerrainVertices[num+1].normal,&tempNormal_1,&tempNormal_2);
		TerrainVertices[num + 1].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, alpha2);//.5
		TerrainVertices[num + 1].tu = 1;
		TerrainVertices[num + 1].tv = 0;


		TerrainVertices[num + 0].position = tempVertex_3;
		//D3DXVec3Cross(&TerrainVertices[num+2].normal,&tempNormal_1,&tempNormal_2);
		TerrainVertices[num + 0].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, alpha3);
		TerrainVertices[num + 0].tu = 1;
		TerrainVertices[num + 0].tv = 1;


		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][num2];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1 + DetailLevel][num2 + DetailLevel];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[num1][num2 + DetailLevel];



		tempY1 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYLow];
		tempY2 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXHigh][edgeYHigh];
		tempY3 = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[edgeXLow][edgeYHigh];


		//Prepare for 2nd triangle of quad
		tempVertex_1 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY1, num2 * 2 * offsetZ + (0) + mapZ);
		tempVertex_2 = D3DXVECTOR3(num1 * 2 * offsetX + (squareSize)+mapX, tempY2, num2 * 2 * offsetZ + (squareSize)+mapZ);
		tempVertex_3 = D3DXVECTOR3(num1 * 2 * offsetX + (0) + mapX, tempY3, num2 * 2 * offsetZ + (squareSize)+mapZ);



		TerrainVertices[num + 4].position = tempVertex_3;
		//D3DXVec3Cross(&TerrainVertices[num+4].normal,&tempNormal_1,&tempNormal_2);

		TerrainVertices[num + 4].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, alpha4);
		TerrainVertices[num + 4].tu = 0;
		TerrainVertices[num + 4].tv = 1;

		num1 += (DetailLevel);


		TerrainVertices[num + 5].position = TerrainVertices[num + 0].position;
		//TerrainVertices[num+5].normal =   TerrainVertices[num+2].normal;

		TerrainVertices[num + 5].color = TerrainVertices[num + 0].color;
		TerrainVertices[num + 5].tu = TerrainVertices[num + 0].tu;
		TerrainVertices[num + 5].tv = TerrainVertices[num + 0].tv;

		TerrainVertices[num + 3].position = TerrainVertices[num + 2].position;
		//TerrainVertices[num+4].normal =   TerrainVertices[num+0].normal;

		TerrainVertices[num + 3].color = TerrainVertices[num + 2].color;
		TerrainVertices[num + 3].tu = TerrainVertices[num + 2].tu;
		TerrainVertices[num + 3].tv = TerrainVertices[num + 2].tv;


		//
		//Model of terrain square showing vertex values (6 needed)
		// 3            5
		//  ************* 2
		//  *          **
		//  *       *   *
		//  *    *      *
		//  * *         *
		//4 *************
		// 0             1
		//

		num += 6;
	}


	NewTerrain_Buffer->Unlock();


	//
	// Create a mesh
	//

	HRESULT hr = 0;

	hr = D3DXCreateMeshFVF((MeshWidth*MeshHeight * 2),
		(MeshWidth*MeshHeight * 6),
		D3DXMESH_VB_MANAGED,
		IMPROVEDVERTEX::FVF_Flags, // This is how each vertex is laid out. 
		Graphics_Device,
		&Temp_Terrain_Mesh);


	IMPROVEDVERTEX* mVertices = NULL;




	Temp_Terrain_Mesh->LockVertexBuffer(0, (void**)&mVertices);
	{
		for (counter = 0; counter<(MeshWidth*MeshHeight * 6); counter++)
		{
			mVertices[counter] = TerrainVertices[counter];
		}

	}
	Temp_Terrain_Mesh->UnlockVertexBuffer();



	WORD* pIndices = NULL;

	Temp_Terrain_Mesh->LockIndexBuffer(0, (void**)&pIndices);
	{
		for (counter = 0; counter<(MeshWidth*MeshHeight * 6); counter++)
		{
			pIndices[counter] = counter;
		}

	}
	Temp_Terrain_Mesh->UnlockIndexBuffer();




	DWORD* pAttributes = NULL;
	Temp_Terrain_Mesh->LockAttributeBuffer(0, &pAttributes);
	{



		//edit the loop below:
		//move the mesh creation to after the loop
		//add a counter in the loop
		//use pAttributes as a temporary buffer and then in another loop fill a new pAttributes whose size equals the counter
		//use this new smaller pAttributes to create the meshes


		int texX, texY;

		for (counter = 0; counter<(MeshWidth*MeshHeight * 2); counter += 2)
		{




			texX = counter / 2;
			texY = 0;
			while (texX >= MeshWidth)
			{
				texX -= MeshWidth;
				texY += DetailLevel;
			}
			texX *= DetailLevel;

			if (texY >= MeshHeight*DetailLevel)
			{
				texY = MeshHeight - DetailLevel;
			}

			//pAttributes[counter] = textureMap[texX][texY];//evens are bottom right triangle
			//pAttributes[counter+1] = textureMap[texX][texY];//odds are top left triangle

			pAttributes[counter] = -1;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY];//evens are bottom right triangle
			pAttributes[counter + 1] = -1;//ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY];//odds are top left triangle

			//ML->NorthSquare(mapIndex, texX, texY);

			int blendEdgeSquare = 0;
			int blendEdgeChunk = 0;

			int blendEdgeX = 0;
			int blendEdgeY = 0;

			std::vector<int> chunkList;
			std::vector<int> squareList;

			//for north south
			if (blendDirection == 0)
			{
				if (texY + DetailLevel < MeshHeight*DetailLevel)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY + DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY + DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY + DetailLevel];//odds are top left triangle
					}
				}
				else//deal with the lower edge
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, 0 * DetailLevel, 1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}
					/*
					blendEdgeChunk = ML->NorthChunk(mapIndex, chunkIndex, texX, texY, chunkList);
					blendEdgeSquare = ML->NorthSquare(mapIndex, chunkIndex, texX, texY, squareList);

					blendEdgeX = texX;
					blendEdgeY = 0;
					*/
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
					{
						pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
					}
				}
			}

			//for south north
			if (blendDirection == 1)
			{
				if (texY - DetailLevel >= 0)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY - DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY - DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY - DetailLevel];//odds are top left triangle
					}
				}
				else//deal with the upper edge
				{


					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, 0 * DetailLevel, -1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}

					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
					{
						pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
					}
				}
			}

			//for east west
			if (blendDirection == 3)
			{
				if (texX + DetailLevel < MeshWidth*DetailLevel)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY];//odds are top left triangle
					}
				}
				else//deal with the left edge
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, 1 * DetailLevel, 0 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}

					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
					{
						pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
					}
				}
			}

			//for west east
			if (blendDirection == 2)
			{
				if (texX - DetailLevel >= 0)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY];//odds are top left triangle
					}
				}
				else//deal with the right edge
				{
					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, -1 * DetailLevel, 0 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}

					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
					{
						pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
					}
				}
			}


			//for north east
			if (blendDirection == 4)
			{
				if (texY + DetailLevel < MeshHeight*DetailLevel && texX + DetailLevel < MeshWidth*DetailLevel)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY + DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY + DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY + DetailLevel];//odds are top left triangle
					}
				}
				else
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, 1 * DetailLevel, 1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}
				}
				if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
				{
					pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
					pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
				}

			}

			//for south west
			if (blendDirection == 5)
			{
				if (texY - DetailLevel >= 0 && texX - DetailLevel >= 0)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY - DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY - DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY - DetailLevel];//odds are top left triangle
					}
				}
				else
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, -1 * DetailLevel, -1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}
				}

				if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
				{
					pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
					pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
				}

			}


			//for south east
			if (blendDirection == 7)
			{
				if (texY - DetailLevel >= 0 && texX + DetailLevel < MeshWidth*DetailLevel)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY - DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY - DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX + DetailLevel][texY - DetailLevel];//odds are top left triangle
					}
				}
				else
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, 1 * DetailLevel, -1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}
				}

				if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
				{
					pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
					pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
				}

			}

			//for north west
			if (blendDirection == 6)
			{
				if (texY + DetailLevel < MeshHeight*DetailLevel && texX - DetailLevel >= 0)//bounds check
				{
					if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY + DetailLevel])
					{
						//set up the subsets based on textures
						pAttributes[counter] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY + DetailLevel];//evens are bottom right triangle
						pAttributes[counter + 1] = ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX - DetailLevel][texY + DetailLevel];//odds are top left triangle
					}
				}
				else
				{

					ConnectingSquares CSEdge;
					CSEdge = ML->FindConnectingTextureSquares(chunkIndex, mapIndex, texX, texY, -1 * DetailLevel, 1 * DetailLevel);
					if (CSEdge.squares.size() > 0)
					{
						blendEdgeChunk = CSEdge.squares[0].chunk;
						blendEdgeSquare = CSEdge.squares[0].square;
						blendEdgeX = CSEdge.squares[0].x;
						blendEdgeY = CSEdge.squares[0].z;
					}
				}
				if (ML->TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[texX][texY] <  ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY])
				{
					pAttributes[counter] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//evens are bottom right triangle
					pAttributes[counter + 1] = ML->TerrainMapChunks[blendEdgeChunk].TerrainMapData[blendEdgeSquare].textureMap[blendEdgeX][blendEdgeY];//odds are top left triangle
				}

			}

		}
	}

	Temp_Terrain_Mesh->UnlockAttributeBuffer();

	if (true)//if Optimization flag is set, do optimizations
	{
		//create adjacency buffer
		LPD3DXBUFFER pAdjacencyBuffer = NULL;
		D3DXCreateBuffer(Temp_Terrain_Mesh->GetNumFaces() * 3, &pAdjacencyBuffer);


		DWORD adjac[(16 + 2)*(16 + 2) * 6];
		Temp_Terrain_Mesh->GenerateAdjacency(0.1f, adjac);
		D3DXComputeNormals(Temp_Terrain_Mesh, adjac);

		// optimize mesh
		hr = Temp_Terrain_Mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |    // Reorders faces to optimize for fewer attribute bundle state changes and enhanced ID3DXBaseMesh::DrawSubset performance.
			D3DXMESHOPT_COMPACT |     // Reorders faces to remove unused vertices and faces.
			D3DXMESHOPT_VERTEXCACHE // Reorders faces to increase the cache hit rate of vertex caches. // or D3DXMESHOPT_STRIPREORDER
			,  // Optimize the faces only; do not optimize the vertices. EDIT - removed this to vastly speed up rendering
			adjac,
			NULL, NULL, NULL);

		if (pAdjacencyBuffer != NULL)
			pAdjacencyBuffer->Release();

	}




	return Temp_Terrain_Mesh;


}


void TerrainMeshData::Release()
{
	NewTerrain_Buffer->Release();
	Temp_Terrain_Mesh->Release();
}
