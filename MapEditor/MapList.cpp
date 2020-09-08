#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include "MapList.h"

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library


//make these into recursive functions, or at least looping functions
//e.g. after each iteration, if the index is still zero, do another iteration with adjusted x and y values


//change this function to return a ConnectingSquares object that contains a vector of COnnectingSquare objects
//this is for the edges of caves, where one square connects to 2 or more on the same edge


//This function is the same as the normal FindConnectingSquares except it doesn't add to the editedterrain vector
//this is to avoid an invinite loop
ConnectingSquares MapList::FindConnectingTextureSquares(int chunkNum, int squareNum, int xStart, int zStart, int xOffset, int zOffset)
{
	ConnectingSquares CSList;

	ConnectingSquare CS;
	CS.chunk = chunkNum;
	CS.square = squareNum;
	CS.x = xStart;
	CS.z = zStart;
	CS.xOffset = xOffset;
	CS.zOffset = zOffset;

	CSList.squares.push_back(CS);

	bool foundSquare = false;


	//repeat until every item in CSList is valid (that is, each x and z are >= 0 and < size)
	while (foundSquare == false)
	{

		int num = 0;
		for (num = 0; num < CSList.squares.size(); num++)
		{
			ConnectingSquare CSTemp;

			CSTemp = CSList.squares[num];

			if (CSTemp.x + CSTemp.xOffset < 0)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);

				int newChunkNum1 = WestChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = WestSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].width + 1;
					CSTemp2.z = CSTemp.z;

					CSTemp2.xOffset = CSTemp.xOffset + (CSTemp.x - 1);
					CSTemp2.zOffset = CSTemp.zOffset;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							//EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 4)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = CSTemp.chunk;
									CE.square = CSTemp.square;
									CE.direction = 4;
									//EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.x + CSTemp.xOffset >= TerrainMapChunks[CSTemp.chunk].TerrainMapData[CSTemp.square].width)
			{
				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = EastChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = EastSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);
				/*
				int newXStart = 0;
				int newZStart = zStart;

				int newXOffset = xOffset - (TerrainMapChunks[chunkNum].TerrainMapData[squareNum].width-xStart);
				int newZOffset = zOffset;
				*/

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = 0;
					CSTemp2.z = CSTemp.z;

					CSTemp2.xOffset = CSTemp.xOffset - (TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].width - CSTemp.x);
					CSTemp2.zOffset = CSTemp.zOffset;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							//EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 2)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = chunkNum;
									CE.square = squareNum;
									CE.direction = 2;
									//EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.z + CSTemp.zOffset < 0)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = SouthChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = SouthSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);
				/*
				int newXStart = xStart;
				int newZStart = TerrainMapChunks[newChunkNum].TerrainMapData[newSquareNum].length+1;//newChunkNum

				int newXOffset = xOffset;
				int newZOffset = zOffset + (zStart-1);
				*/

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = CSTemp.x;
					CSTemp2.z = TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].length + 1;

					CSTemp2.xOffset = CSTemp.xOffset;
					CSTemp2.zOffset = CSTemp.zOffset + (CSTemp.z - 1);;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							//EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 3)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = chunkNum;
									CE.square = squareNum;
									CE.direction = 3;
									//EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.z + CSTemp.zOffset >= TerrainMapChunks[CSTemp.chunk].TerrainMapData[CSTemp.square].length)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = NorthChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = NorthSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);

				/*
				int newXStart = xStart;
				int newZStart = 0;

				int newXOffset = xOffset;
				int newZOffset = zOffset - (TerrainMapChunks[chunkNum].TerrainMapData[squareNum].length-zStart);
				*/

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = CSTemp.x;
					CSTemp2.z = 0;

					CSTemp2.xOffset = CSTemp.xOffset;
					CSTemp2.zOffset = CSTemp.zOffset - (TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].length - CSTemp.z);


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							//EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 1)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = CSTemp.chunk;
									CE.square = CSTemp.square;
									CE.direction = 1;
									//EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
		}//end for loop
		bool listComplete = true;

		for (num = 0; num < CSList.squares.size(); num++)
		{
			//check all 4 conditions
			if (CSList.squares[num].x + CSList.squares[num].xOffset < 0)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].x + CSList.squares[num].xOffset >= TerrainMapChunks[CSList.squares[num].chunk].TerrainMapData[CSList.squares[num].square].width)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].z + CSList.squares[num].zOffset < 0)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].z + CSList.squares[num].zOffset >= TerrainMapChunks[CSList.squares[num].chunk].TerrainMapData[CSList.squares[num].square].length)
			{
				listComplete = false;
			}
			else
			{
				//square is finished, so combine the start and offset values
				CSList.squares[num].x += CSList.squares[num].xOffset;
				CSList.squares[num].z += CSList.squares[num].zOffset;
				CSList.squares[num].xOffset = 0;
				CSList.squares[num].zOffset = 0;
			}
		}
		if (listComplete)
		{
			foundSquare = true;
		}
	}//end while loop

	return CSList;


}


ConnectingSquares MapList::FindConnectingSquares(int chunkNum, int squareNum, int xStart, int zStart, int xOffset, int zOffset)
{
	ConnectingSquares CSList;

	ConnectingSquare CS;
	CS.chunk = chunkNum;
	CS.square = squareNum;
	CS.x = xStart;
	CS.z = zStart;
	CS.xOffset = xOffset;
	CS.zOffset = zOffset;

	CSList.squares.push_back(CS);

	bool foundSquare = false;


	//repeat until every item in CSList is valid (that is, each x and z are >= 0 and < size)
	while (foundSquare == false)
	{

		int num = 0;
		for (num = 0; num < CSList.squares.size(); num++)
		{
			ConnectingSquare CSTemp;

			CSTemp = CSList.squares[num];

			if (CSTemp.x + CSTemp.xOffset < 0)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);

				int newChunkNum1 = WestChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = WestSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].width + 1;
					CSTemp2.z = CSTemp.z;

					CSTemp2.xOffset = CSTemp.xOffset + (CSTemp.x - 1);
					CSTemp2.zOffset = CSTemp.zOffset;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 4)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = CSTemp.chunk;
									CE.square = CSTemp.square;
									CE.direction = 4;
									EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.x + CSTemp.xOffset >= TerrainMapChunks[CSTemp.chunk].TerrainMapData[CSTemp.square].width)
			{
				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = EastChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = EastSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);

				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = 0;
					CSTemp2.z = CSTemp.z;

					CSTemp2.xOffset = CSTemp.xOffset - (TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].width - CSTemp.x);
					CSTemp2.zOffset = CSTemp.zOffset;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 2)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = chunkNum;
									CE.square = squareNum;
									CE.direction = 2;
									EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.z + CSTemp.zOffset < 0)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = SouthChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = SouthSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);


				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = CSTemp.x;
					CSTemp2.z = TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].length + 1;

					CSTemp2.xOffset = CSTemp.xOffset;
					CSTemp2.zOffset = CSTemp.zOffset + (CSTemp.z - 1);;


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 3)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = chunkNum;
									CE.square = squareNum;
									CE.direction = 3;
									EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
			else if (CSTemp.z + CSTemp.zOffset >= TerrainMapChunks[CSTemp.chunk].TerrainMapData[CSTemp.square].length)
			{

				std::vector<int> chunkList;
				std::vector<int> squareList;

				//remove current square
				CSList.squares.erase(CSList.squares.begin() + num);


				int newChunkNum1 = NorthChunk(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, chunkList);
				int newSquareNum1 = NorthSquare(CSTemp.square, CSTemp.chunk, CSTemp.x, CSTemp.z, squareList);


				//push new square or squares
				for (int num2 = 0; num2 < chunkList.size(); num2++)
				{
					ConnectingSquare CSTemp2;

					CSTemp2.chunk = chunkList[num2];
					CSTemp2.square = squareList[num2];

					CSTemp2.x = CSTemp.x;
					CSTemp2.z = 0;

					CSTemp2.xOffset = CSTemp.xOffset;
					CSTemp2.zOffset = CSTemp.zOffset - (TerrainMapChunks[CSTemp2.chunk].TerrainMapData[CSTemp2.square].length - CSTemp.z);


					bool cont = false;
					int newChunkNum = CSTemp2.chunk;
					int newSquareNum = CSTemp2.square;

					if (newChunkNum != 0 && newSquareNum != 0)
					{
						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								cont = true;
							}
						}
						if (cont == false)
						{
							EditedSquare ES;
							ES.square = newSquareNum;
							ES.chunk = newChunkNum;
							EditedSquares.push_back(ES);
						}

						for (int n = 0; n < EditedSquares.size(); n++)
						{
							if (EditedSquares[n].square == newSquareNum && EditedSquares[n].chunk == newChunkNum)
							{
								bool containsEdge = false;
								for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
								{
									if (EditedSquares[n].ConnectingEdges[n2].chunk == CSTemp.chunk
										&& EditedSquares[n].ConnectingEdges[n2].square == CSTemp.square
										&& EditedSquares[n].ConnectingEdges[n2].direction == 1)
									{
										containsEdge = true;
									}

								}
								if (containsEdge == false)
								{
									ConnectingEdge CE;
									CE.chunk = CSTemp.chunk;
									CE.square = CSTemp.square;
									CE.direction = 1;
									EditedSquares[n].ConnectingEdges.push_back(CE);
								}
							}


						}
						CSList.squares.push_back(CSTemp2);
					}
				}


			}
		}//end for loop
		bool listComplete = true;

		for (num = 0; num < CSList.squares.size(); num++)
		{
			//check all 4 conditions
			if (CSList.squares[num].x + CSList.squares[num].xOffset < 0)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].x + CSList.squares[num].xOffset >= TerrainMapChunks[CSList.squares[num].chunk].TerrainMapData[CSList.squares[num].square].width)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].z + CSList.squares[num].zOffset < 0)
			{
				listComplete = false;
			}
			else if (CSList.squares[num].z + CSList.squares[num].zOffset >= TerrainMapChunks[CSList.squares[num].chunk].TerrainMapData[CSList.squares[num].square].length)
			{
				listComplete = false;
			}
			else
			{
				//square is finished, so combine the start and offset values
				CSList.squares[num].x += CSList.squares[num].xOffset;
				CSList.squares[num].z += CSList.squares[num].zOffset;
				CSList.squares[num].xOffset = 0;
				CSList.squares[num].zOffset = 0;
			}
		}
		if (listComplete)
		{
			foundSquare = true;
		}
	}//end while loop

	return CSList;

}




//IMPORTANT: change so this is no longer a recursive function, but rather a simple looping function


ConnectingSquare MapList::FindConnectingSquare(ConnectingSquares& CSList, int chunkNum, int squareNum, int xStart, int zStart, int xOffset, int zOffset)
{

	//ConnectingSquares CSList;


	ConnectingSquare CS;

	bool foundSquare = false;

	std::vector<int> chunkList;
	std::vector<int> squareList;
	int djsfhhjd = 0;
	while (!foundSquare)
	{

		if (xStart + xOffset < 0)
		{

			//change these to return a vector of ints for all connected squares
			int newChunkNum = WestChunk(squareNum, chunkNum, xStart, zStart, chunkList);
			int newSquareNum = WestSquare(squareNum, chunkNum, xStart, zStart, squareList);

			int newXStart = TerrainMapChunks[newChunkNum].TerrainMapData[newSquareNum].width + 1;//newChunkNum
			int newZStart = zStart;

			int newXOffset = xOffset + (xStart - 1);
			int newZOffset = zOffset;


			bool cont = false;

			if (newChunkNum != 0 && newSquareNum != 0)
			{
				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						cont = true;
					}
				}
				if (cont == false)
				{
					EditedSquare ES;
					ES.square = newSquareNum;
					ES.chunk = newChunkNum;
					EditedSquares.push_back(ES);
				}

				//note: all connect edges loops need to be called only the outside of the rx ry loop in the edit method
				//here we need to set some flag telling which edited squares should be connected and in which direction
				//ConnectEdges(chunkNum, squareNum, newChunkNum, newSquareNum, 4);

				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						bool containsEdge = false;
						for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
						{
							if (EditedSquares[n].ConnectingEdges[n2].chunk == chunkNum
								&& EditedSquares[n].ConnectingEdges[n2].square == squareNum
								&& EditedSquares[n].ConnectingEdges[n2].direction == 4)
							{
								containsEdge = true;
							}

						}
						if (containsEdge == false)
						{
							ConnectingEdge CE;
							CE.chunk = chunkNum;
							CE.square = squareNum;
							CE.direction = 4;
							EditedSquares[n].ConnectingEdges.push_back(CE);
						}
					}
				}


				return FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);
				//FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);
			}



			foundSquare = true;
		}
		if (xStart + xOffset >= TerrainMapChunks[chunkNum].TerrainMapData[squareNum].width)
		{

			int newChunkNum = EastChunk(squareNum, chunkNum, xStart, zStart, chunkList);
			int newSquareNum = EastSquare(squareNum, chunkNum, xStart, zStart, squareList);

			int newXStart = 0;
			int newZStart = zStart;

			int newXOffset = xOffset - (TerrainMapChunks[chunkNum].TerrainMapData[squareNum].width - xStart);
			int newZOffset = zOffset;


			bool cont = false;

			if (newChunkNum != 0 && newSquareNum != 0)
			{
				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						cont = true;
					}
				}
				if (cont == false)
				{
					EditedSquare ES;
					ES.square = newSquareNum;
					ES.chunk = newChunkNum;
					EditedSquares.push_back(ES);
				}

				//ConnectEdges(chunkNum, squareNum, newChunkNum, newSquareNum, 2);

				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						bool containsEdge = false;
						for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
						{
							if (EditedSquares[n].ConnectingEdges[n2].chunk == chunkNum
								&& EditedSquares[n].ConnectingEdges[n2].square == squareNum
								&& EditedSquares[n].ConnectingEdges[n2].direction == 2)
							{
								containsEdge = true;
							}

						}
						if (containsEdge == false)
						{
							ConnectingEdge CE;
							CE.chunk = chunkNum;
							CE.square = squareNum;
							CE.direction = 2;
							EditedSquares[n].ConnectingEdges.push_back(CE);
						}
					}
				}

				int djshfkjfg = 0;

				return FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);
			}
			foundSquare = true;
		}
		if (zStart + zOffset < 0)
		{
			int dsjkfhsd = 0;
			//this method as implemented is correct, we just need to add something to connect the high edge of the south square
			//to the low edge of the north square
			int newChunkNum = SouthChunk(squareNum, chunkNum, xStart, zStart, chunkList);
			int newSquareNum = SouthSquare(squareNum, chunkNum, xStart, zStart, squareList);

			int newXStart = xStart;
			int newZStart = TerrainMapChunks[newChunkNum].TerrainMapData[newSquareNum].length + 1;//newChunkNum

			int newXOffset = xOffset;
			int newZOffset = zOffset + (zStart - 1);


			bool cont = false;

			if (newChunkNum != 0 && newSquareNum != 0)
			{
				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						cont = true;
					}
				}
				if (cont == false)
				{
					EditedSquare ES;
					ES.square = newSquareNum;
					ES.chunk = newChunkNum;
					EditedSquares.push_back(ES);
				}

				//ConnectEdges(chunkNum, squareNum, newChunkNum, newSquareNum, 3);

				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						bool containsEdge = false;
						for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
						{
							if (EditedSquares[n].ConnectingEdges[n2].chunk == chunkNum
								&& EditedSquares[n].ConnectingEdges[n2].square == squareNum
								&& EditedSquares[n].ConnectingEdges[n2].direction == 3)
							{
								containsEdge = true;
							}

						}
						if (containsEdge == false)
						{
							ConnectingEdge CE;
							CE.chunk = chunkNum;
							CE.square = squareNum;
							CE.direction = 3;
							EditedSquares[n].ConnectingEdges.push_back(CE);
						}
					}
				}



				return FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);
				//FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);

			}




			foundSquare = true;

		}
		if (zStart + zOffset >= TerrainMapChunks[chunkNum].TerrainMapData[squareNum].length)
		{
			int newChunkNum = NorthChunk(squareNum, chunkNum, xStart, zStart, chunkList);
			int newSquareNum = NorthSquare(squareNum, chunkNum, xStart, zStart, squareList);

			int newXStart = xStart;
			int newZStart = 0;

			int newXOffset = xOffset;
			int newZOffset = zOffset - (TerrainMapChunks[chunkNum].TerrainMapData[squareNum].length - zStart);


			bool cont = false;

			if (newChunkNum != 0 && newSquareNum != 0)
			{

				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						cont = true;
					}
				}
				if (cont == false)
				{
					EditedSquare ES;
					ES.square = newSquareNum;
					ES.chunk = newChunkNum;
					EditedSquares.push_back(ES);
				}

				//ConnectEdges(chunkNum, squareNum, newChunkNum, newSquareNum, 1);

				for (int n = 0; n < EditedSquares.size(); n++)
				{
					if (EditedSquares[n].square == newSquareNum)
					{
						bool containsEdge = false;
						for (int n2 = 0; n2 < EditedSquares[n].ConnectingEdges.size(); n2++)
						{
							if (EditedSquares[n].ConnectingEdges[n2].chunk == chunkNum
								&& EditedSquares[n].ConnectingEdges[n2].square == squareNum
								&& EditedSquares[n].ConnectingEdges[n2].direction == 1)
							{
								containsEdge = true;
							}

						}
						if (containsEdge == false)
						{
							ConnectingEdge CE;
							CE.chunk = chunkNum;
							CE.square = squareNum;
							CE.direction = 1;
							EditedSquares[n].ConnectingEdges.push_back(CE);
						}
					}
				}

				return FindConnectingSquare(CSList, newChunkNum, newSquareNum, newXStart, newZStart, newXOffset, newZOffset);
			}
			foundSquare = true;
		}
		else
		{
			if (CSList.squares.size() == 0)
			{
				ConnectingSquare CSTemp;
				CSTemp.chunk = chunkNum;
				CSTemp.square = squareNum;
				CSTemp.x = xStart + xOffset;
				CSTemp.z = zStart + zOffset;
				CSList.squares.push_back(CSTemp);
			}

			foundSquare = true;
		}



	}



	CS.chunk = chunkNum;
	CS.square = squareNum;
	CS.x = xStart + xOffset;
	CS.z = zStart + zOffset;

	//CSList.squares.push_back(CS);

	return CS;
}

VOID MapList::ConnectEdges(int chunkNum1, int squareNum1, int chunkNum2, int squareNum2, int direction)
{

	if (chunkNum1 != 0 && chunkNum2 != 0)
	{


		if (direction == 1)//square2 is north of square1
		{
			int offset1 = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].position.x - TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].position.x;


			int lowMatch = 0;
			int highMatch = 0;


			int n2 = 0;

			int squareSize = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].length;

			for (int n1 = offset1; n1 <= TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].width; n1++)
			{

				if (n1 >= 0)
				{
					TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].heightMap[n2][squareSize] = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].heightMap[n1][0];
				}
				n2++;
			}
		}

		if (direction == 2)//square2 is east of square1
		{
			int offset1 = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].position.z - TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].position.z;


			int lowMatch = 0;
			int highMatch = 0;


			int n2 = 0;

			int squareSize = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].width;

			for (int n1 = offset1; n1 <= TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].length; n1++)
			{

				if (n1 >= 0)
				{
					TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].heightMap[squareSize][n2] = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].heightMap[0][n1];
				}
				n2++;
			}
		}

		if (direction == 3)//square2 is south of square1
		{
			int offset1 = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].position.x - TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].position.x;


			int lowMatch = 0;
			int highMatch = 0;


			int n2 = 0;

			int squareSize = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].length;

			for (int n1 = offset1; n1 <= TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].width; n1++)
			{

				if (n1 >= 0)
				{
					TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].heightMap[n2][squareSize] = TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].heightMap[n1][0];
				}
				n2++;
			}
		}


		if (direction == 4)//square2 is west of square1
		{
			int offset1 = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].position.z - TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].position.z;


			int lowMatch = 0;
			int highMatch = 0;


			int n2 = 0;

			int squareSize = TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].width;

			for (int n1 = offset1; n1 <= TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].length; n1++)
			{

				if (n1 >= 0)
				{
					TerrainMapChunks[chunkNum2].TerrainMapData[squareNum2].heightMap[squareSize][n2] = TerrainMapChunks[chunkNum1].TerrainMapData[squareNum1].heightMap[0][n1];
				}
				n2++;
			}
		}



	}

}

//these methods must all be changed to return a vector of ints, so that multiple connecting edges can be returned
int MapList::NorthSquare(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'n')//if direction == north
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::EastSquare(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	int sjahdsjkd = 0;
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'e')//if direction == east
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::SouthSquare(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 's')//if direction == south
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::WestSquare(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'w')//if direction == west
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}

/////////////////////////////////////
int MapList::NorthChunk(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'n')//if direction == north
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);

			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::EastChunk(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'e')//if direction == east
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::SouthChunk(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 's')//if direction == south
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
int MapList::WestChunk(int MeshNumber, int ChunkNumber, int x, int y, std::vector<int>& squareList)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'w')//if direction == west
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			//return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
			squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
			//}
		}
	}
	if (squareList.size() > 0)
	{
		return squareList[0];
	}
	return 0;
}
/////////////////////////////////////
//these methods must all be changed to return a vector of ints, so that multiple connecting edges can be returned
//note: maybe call the quick versions first, but have them return -1 if there are multiple squares
//then if -1 is returned, we can call the more time consuming vector version of this function
int MapList::NorthSquareQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'n')//if direction == north
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
																								//squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
																								//}
		}
	}

	return 0;
}
int MapList::EastSquareQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	int sjahdsjkd = 0;
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'e')//if direction == east
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
																								//squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
																								//}
		}
	}

	return 0;
}
int MapList::SouthSquareQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 's')//if direction == south
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
																								//squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
																								//}
		}
	}

	return 0;
}
int MapList::WestSquareQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'w')//if direction == west
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum;//return index of the TerrainSquare
																								//squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].squareNum);
																								//}
		}
	}

	return 0;
}

/////////////////////////////////////
/////////////////////////////////////
int MapList::NorthChunkQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'n')//if direction == north
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
																							   //squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);

																							   //}
		}
	}
	return 0;
}
int MapList::EastChunkQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'e')//if direction == east
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
																							   //squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
																							   //}
		}
	}

	return 0;
}
int MapList::SouthChunkQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 's')//if direction == south
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= x && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= x)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
																							   //squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
																							   //}
		}
	}

	return 0;
}
int MapList::WestChunkQuick(int MeshNumber, int ChunkNumber, int x, int y)
{
	for (int nS = 0; nS < TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges.size(); nS++)
	{
		if (TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].direction == 'w')//if direction == west
		{
			//if(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].leftIndex <= y && TerrainMapChunks[chunkIndex].TerrainMapData[MeshNumber].edges[nS].leftIndex >= y)//if x is between the indices
			//{
			return TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum;//return index of the TerrainSquare
																							   //squareList.push_back(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].edges[nS].chunkNum);
																							   //}
		}
	}

	return 0;
}
/////////////////////////////////////


void MapList::EditHeightMap(int MeshNumber, int ChunkNumber, int x, int y, float magnitude, float radius, float level, int tool, int textureSelected, int materialSelected, int textureDepth, LPDIRECT3DDEVICE9 Graphics_Device)
{




	//temp variables needed for transitions between 2 big squares
	int EHM;
	int EHC;
	int EHX;
	int EHY;
	//temp variables needed for transitions between 2 big squares
	int RHM;
	int RHC;
	int RHX;
	int RHY;
	//temp variables needed for transitions between 2 big squares
	int RHM1;
	int RHM2;
	int RHC1;
	int RHC2;
	int RHX1;
	int RHY1;
	int RHM3;
	int RHM4;
	int RHC3;
	int RHC4;
	int RHX2;
	int RHY2;

	int dx;
	int dy;

	float distance = 0;

	float maxDiff = 0.0f;
	int maxDiffX = 0.0f;
	int maxDiffY = 0.0f;

	bool cont = false;
	//vector<int>EditedSquares;
	EditedSquare editedSquare;
	editedSquare.square = MeshNumber;
	editedSquare.chunk = ChunkNumber;
	EditedSquares.push_back(editedSquare);

	for (int rx = -1 * radius; rx <= radius; rx++)
	{
		for (int ry = -1 * radius; ry <= radius; ry++)
		{

			EHM = MeshNumber;
			EHC = ChunkNumber;
			EHX = x + rx;
			EHY = y + ry;

			//FindConnectingSquare(int chunkNum, int squareNum, int xStart, int zStart, int xOffset, int zOffset)

			ConnectingSquares CSList;
			//ConnectingSquare CS = FindConnectingSquare(CSList, ChunkNumber, MeshNumber, x, y, rx, ry);

			//CSList.squares.clear();

			CSList = FindConnectingSquares(ChunkNumber, MeshNumber, x, y, rx, ry);

			if (CSList.squares.size() < 1)
			{
				int dslkg = 0;
			}

			int jdshfskjd = 0;

			/*
			EHC = CS.chunk;
			EHM = CS.square;
			EHX = CS.x;
			EHY = CS.z;
			*/



			if (EHC == 0 || EHM == 0)
			{
				EHX = 0;
				EHY = 0;
			}

			while (CSList.squares.size() > 0)
			{

				if (CSList.squares.back().chunk != 0 && CSList.squares.back().square != 0)
				{
					EHC = CSList.squares.back().chunk;
					EHM = CSList.squares.back().square;
					EHX = CSList.squares.back().x;
					EHY = CSList.squares.back().z;
				}
				/*
				//temp fix
				if(EHX < 0)
				{
				EHC = 0;
				EHM = 0;
				EHX = 0;
				}
				if(EHY < 0)
				{
				EHC = 0;
				EHM = 0;
				EHY = 0;
				}
				*/
				dx = rx;
				dy = ry;

				//insert this into the while loops below in case of squares with different lengths or widths (use EHM)
				int width = TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].width + 1;
				int length = TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].length + 1;

				cont = false;


				/*
				while(EHY >= length)
				{
				EHM = NorthSquare(EHM, EHC, EHX, EHY);
				EHY = EHY - length;
				dy-=1;

				for(int n = 0; n < EditedSquares.size(); n++)
				{
				if(EditedSquares[n] == EHM)
				{
				cont = true;
				}
				}
				if(cont == false)
				{
				EditedSquares.push_back(EHM);
				}
				}
				cont = false;
				while(EHY < 0)
				{
				EHM = SouthSquare(EHM, EHC, EHX, EHY);
				EHY = length + EHY;
				dy+=1;

				for(int n = 0; n < EditedSquares.size(); n++)
				{
				if(EditedSquares[n] == EHM)
				{
				cont = true;
				}
				}
				if(cont == false)
				{
				EditedSquares.push_back(EHM);
				}
				}

				cont = false;
				if(EHX >= (width))
				{
				EHM = EastSquare(EHM, EHC, EHX, EHY);
				EHX = EHX - width;
				dx-=1;

				for(int n = 0; n < EditedSquares.size(); n++)
				{
				if(EditedSquares[n] == EHM)
				{
				cont = true;
				}
				}
				if(cont == false)
				{
				EditedSquares.push_back(EHM);
				}
				}
				cont = false;
				while(EHX < 0)
				{
				EHM = WestSquare(EHM, EHC, EHX, EHY);
				EHX = width + EHX;
				dx+=1;

				for(int n = 0; n < EditedSquares.size(); n++)
				{
				if(EditedSquares[n] == EHM)
				{
				cont = true;
				}
				}
				if(cont == false)
				{
				EditedSquares.push_back(EHM);
				}
				}
				*/

				//this might be a problem...

				//if the mesh is not visible, create it in order to prevent an error
				if (TerrainMapChunks[EHC].TerrainMapData[EHM].visible == false)
				{
					//at this point we are simply creating the mesh at detail level 2, really this could be any detail level we want
					if (TerrainMapChunks[EHC].TerrainMapData[EHM].Terrain_Mesh == NULL || TerrainMapChunks[EHC].TerrainMapData[EHM].detailLevel != 2)
					{
						TerrainMeshData *TMDD2 = new TerrainMeshData(Graphics_Device);

						newMesh = TMDD2->GetTerrainMesh(this, EHC, EHM, 2, true);
						if (TerrainMapChunks[EHC].TerrainMapData[EHM].Terrain_Mesh != NULL)
						{
							TerrainMapChunks[EHC].TerrainMapData[EHM].Terrain_Mesh->Release();
						}
						if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[EHC].TerrainMapData[EHM].Terrain_Mesh))) {
						}
						TMDD2->Release();
						//create target mesh
						newMesh = TMDD2->GetTerrainMesh(this, EHC, EHM, 2, false);
						if (TerrainMapChunks[EHC].TerrainMapData[EHM].Target_Terrain_Mesh != NULL)
						{
							TerrainMapChunks[EHC].TerrainMapData[EHM].Target_Terrain_Mesh->Release();
						}
						if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[EHC].TerrainMapData[EHM].Target_Terrain_Mesh))) {
						}
						/*
						for(int bm = 0; bm < 8; bm ++)
						{

						//create drawing mesh
						newMesh = TMDD2->GetTerrainBlendingMesh(this, EHM, 1, bm);
						if(TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm] != NULL)
						{
						TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm]->Release();
						}
						if (FAILED(newMesh->CloneMeshFVF(0,D3DFVF_IMPROVEDVERTEX,Graphics_Device,&TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm]))) {
						}

						TMDD2->Release();

						}
						*/
						TMDD2->Release();
						delete TMDD2;

						TerrainMapChunks[EHC].TerrainMapData[EHM].detailLevel = 2;
					}
				}



				if (tool == 1)//raise or lower terrain (mountain)
				{

					//this algorithm is very good at creating realistic looking mountains because it doesn't raise the terrain evenly
					//but rather it raises it by a disproportionate amount in the middle and around the edges
					//it doesn't create a perfect bell shape, it creates something that looks much more natural

					distance = sqrt((float)((dx*dx) + (dy*dy)));
					if (abs(distance) <= abs(radius))
					{
						if (distance < 1)
						{
							distance = 1;
						}
						TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] += magnitude*sqrt(radius)*((1 / sqrt(distance)) - (1 / sqrt(radius)));


					}
				}

				if (tool == 2)//raise or lower terrain (hill)
				{


					distance = sqrt((float)((dx*dx) + (dy*dy)));
					if (abs(distance) <= abs(radius))
					{
						if (distance < 1)
						{
							distance = 1;
						}
						TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] += magnitude*.05*(radius - distance);
					}
				}

				if (tool == 3)//level terrain
				{

					magnitude = abs(magnitude);

					distance = sqrt((float)((dx*dx) + (dy*dy)));
					if (abs(distance) <= abs(radius))
					{
						if (distance < 1)
						{
							distance = 1;
						}
						if (TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] > level)
						{

							if (TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] - magnitude*(radius - distance) <= level)
							{
								TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] = level;
							}
							else
							{
								TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] -= magnitude*(radius - distance);
							}

						}
						else
						{
							if (TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] + magnitude*(radius - distance) >= level)
							{
								TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] = level;
							}
							else
							{
								TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] += magnitude*(radius - distance);
							}

						}
						//heightMap[MeshNumber][x + rx][y + ry]=0;
					}
				}

				if (tool == 4)//smooth terrain - makes the distance from each point to the surrounding points uniform accross the whole area
				{


					distance = sqrt((float)((dx*dx) + (dy*dy)));
					if (abs(distance) <= abs(radius))
					{
						if (distance < 1)
						{
							distance = 1;
						}
						ConnectingSquare CS2;


						RHM1 = EHM;
						RHM2 = EHM;

						RHM3 = EHM;
						RHM4 = EHM;

						//temp initializations
						RHC1 = 1;
						RHC2 = 1;

						RHC3 = 1;
						RHC4 = 1;

						RHX1 = EHX - 1;
						RHX2 = EHX + 1;
						RHY1 = EHY - 1;
						RHY2 = EHY + 1;

						ConnectingSquares CSList1;
						CS2 = FindConnectingSquare(CSList1, EHC, EHM, EHX, EHY, -1, 0);
						CSList1 = FindConnectingSquares(EHC, EHM, EHX, EHY, -1, 0);
						RHC1 = CS2.chunk;
						RHM1 = CS2.square;
						RHX1 = CS2.x;


						ConnectingSquares CSList2;
						CS2 = FindConnectingSquare(CSList2, EHC, EHM, EHX, EHY, 1, 0);
						CSList2 = FindConnectingSquares(EHC, EHM, EHX, EHY, 1, 0);
						RHC2 = CS2.chunk;
						RHM2 = CS2.square;
						RHX2 = CS2.x;


						float combinedSlope1 = 0;
						float combinedSlope2 = 0;

						for (int csNum = 0; csNum < CSList1.squares.size(); csNum++)
						{
							RHC1 = CSList1.squares[csNum].chunk;
							RHM1 = CSList1.squares[csNum].square;
							RHX1 = CSList1.squares[csNum].x;
							combinedSlope1 += TerrainMapChunks[RHC1].TerrainMapData[RHM1].heightMap[RHX1][EHY];
						}
						combinedSlope1 /= ((float)CSList1.squares.size());

						for (int csNum = 0; csNum < CSList2.squares.size(); csNum++)
						{
							RHC2 = CSList2.squares[csNum].chunk;
							RHM2 = CSList2.squares[csNum].square;
							RHX2 = CSList2.squares[csNum].x;
							combinedSlope2 += TerrainMapChunks[RHC2].TerrainMapData[RHM2].heightMap[RHX2][EHY];
						}
						combinedSlope2 /= ((float)CSList2.squares.size());

						//float slopeX = (TerrainMapChunks[RHC1].TerrainMapData[RHM1].heightMap[RHX1][EHY] - TerrainMapChunks[RHC2].TerrainMapData[RHM2].heightMap[RHX2][EHY])/2;
						//float combinedSlope = (TerrainMapChunks[RHC2].TerrainMapData[RHM2].heightMap[RHX2][EHY] + slopeX);

						float slopeX = (combinedSlope1 - combinedSlope2) / 2;
						float combinedSlope = combinedSlope2 + slopeX;
						///////////////////////////////////


						ConnectingSquares CSList3;
						CS2 = FindConnectingSquare(CSList3, EHC, EHM, EHX, EHY, 0, -1);
						CSList3 = FindConnectingSquares(EHC, EHM, EHX, EHY, 0, -1);
						RHC3 = CS2.chunk;
						RHM3 = CS2.square;

						RHY1 = CS2.z;

						ConnectingSquares CSList4;
						CS2 = FindConnectingSquare(CSList4, EHC, EHM, EHX, EHY, 0, 1);
						CSList4 = FindConnectingSquares(EHC, EHM, EHX, EHY, 0, 1);
						RHC4 = CS2.chunk;
						RHM4 = CS2.square;

						RHY2 = CS2.z;



						float combinedSlope3 = 0;
						float combinedSlope4 = 0;


						for (int csNum = 0; csNum < CSList3.squares.size(); csNum++)
						{
							RHC3 = CSList3.squares[csNum].chunk;
							RHM3 = CSList3.squares[csNum].square;
							RHY1 = CSList3.squares[csNum].z;
							combinedSlope3 += TerrainMapChunks[RHC3].TerrainMapData[RHM3].heightMap[EHX][RHY1];
						}
						combinedSlope3 /= ((float)CSList3.squares.size());

						for (int csNum = 0; csNum < CSList4.squares.size(); csNum++)
						{
							RHC4 = CSList4.squares[csNum].chunk;
							RHM4 = CSList4.squares[csNum].square;
							RHY2 = CSList4.squares[csNum].z;
							combinedSlope4 += TerrainMapChunks[RHC4].TerrainMapData[RHM4].heightMap[EHX][RHY2];
						}
						combinedSlope4 /= ((float)CSList4.squares.size());

						//float slopeY = (TerrainMapChunks[RHC3].TerrainMapData[RHM3].heightMap[EHX][RHY1] - TerrainMapChunks[RHC4].TerrainMapData[RHM4].heightMap[EHX][RHY2])/2;
						//combinedSlope += (TerrainMapChunks[RHC4].TerrainMapData[RHM4].heightMap[EHX][RHY2] + slopeY);

						float slopeY = (combinedSlope3 - combinedSlope4) / 2;
						combinedSlope += (combinedSlope4 + slopeY);



						combinedSlope /= 2;



						if (abs(combinedSlope) < 1000)//temp fix for leveling infinite distances off the map edge
						{
							TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] = (TerrainMapChunks[EHC].TerrainMapData[EHM].heightMap[EHX][EHY] * abs(magnitude) + combinedSlope) / (abs(magnitude) + 1);
						}


					}
				}
				if (tool == 5)//delete objects
				{
					

				}


				if (tool == 6)//edit textures
				{
					int TextureSelected = 0;
					distance = sqrt((float)((dx*dx) + (dy*dy)));
					if (abs(distance) <= abs(radius))
					{
						TerrainMapChunks[EHC].TerrainMapData[EHM].textureMap[EHX][EHY] = textureSelected;

					}
				}

				if (tool == 7)//do nothing (for cleaning up mismatched edges)
				{


				}




				CSList.squares.pop_back();
			}


		}
	}

	//click on a 
	if (tool == 9)//add new map squares
	{
		//when this tool is selected, it creates a new map square on the nearest edge of whatever square you click on
		//for now, assume that the new square has the same dimentions as the one you click on

		//side refers to whichever side of the terrain square is closest and not already bordering another square
		int side = 0;

		//distance to each side
		int distanceN = TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].length - y;
		int distanceE = TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].width - x;
		int distanceS = y;
		int distanceW = x;

		//find the smallest distance
		if (distanceN < distanceS)
		{
			if (distanceE < distanceW)
			{
				if (distanceN < distanceE)
				{
					side = 1;
				}
				else
				{
					side = 2;
				}
			}
			else
			{
				if (distanceN < distanceW)
				{
					side = 1;
				}
				else
				{
					side = 4;
				}
			}
		}
		else
		{
			if (distanceE < distanceW)
			{
				if (distanceS < distanceE)
				{
					side = 3;
				}
				else
				{
					side = 2;
				}
			}
			else
			{
				if (distanceS < distanceW)
				{
					side = 3;
				}
				else
				{
					side = 4;
				}
			}
		}



		NewSquare(TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].length, TerrainMapChunks[ChunkNumber].TerrainMapData[MeshNumber].width, MeshNumber, side, ChunkNumber, Graphics_Device);
	}


	//also add another tool for un-attaching two squares
	if (tool == 10)//connect map squares
	{
		//this tool works by clicking on one square, releasing, and then clicking on another square
		//this action then creates EdgeData for each of the two affected squares if it does not already exist
		//the tool will automatically determine which edges to create (notrth/south/east/west) since there will only
		//be at most one geometric possibility for two squares to connect to each other

		if (activeSquare == 0 && activeChunk == 0)//if first square has been clicked
		{
			activeSquare = MeshNumber;
			activeChunk = ChunkNumber;
		}
		else//second square has been clicked
		{
			ConnectSquares(activeChunk, activeSquare, ChunkNumber, MeshNumber);
		}

	}
	else
	{
		//if else set the stored square anc chunk to 0
		activeSquare = 0;
		activeChunk = 0;
	}


	bool updateTextures = false;
	if (tool == 6 || tool == 7)
	{
		updateTextures = true;
	}

	//note: revise the following for loop to do the following:
	//search through EditedSquares and identify all pairs of neighboring squares
	//call ConnectEdges for each of these pairs
	for (int n = 0; n < EditedSquares.size(); n++)
	{
		while (!EditedSquares[n].ConnectingEdges.empty())
		{
			//ConnectEdges(EditedSquares[n].ConnectingEdges.back().chunk, EditedSquares[n].ConnectingEdges.back().square, EditedSquares[n].chunk, EditedSquares[n].square, EditedSquares[n].ConnectingEdges.back().direction);

			EditedSquares[n].ConnectingEdges.pop_back();
		}
	}

	//bruet force approach: Connect Edges for every edge of every active square
	//(note: that we only need to consider half of the edges (the high edges, not the low ones))
	for (int n = 0; n < EditedSquares.size(); n++)
	{

		//if(direction == 1)//square2 is north of square1
		//if(direction == 2)//square2 is east of square1
		//if(direction == 3)//square2 is south of square1
		//if(direction == 4)//square2 is west of square1

		int eChunk = EditedSquares[n].chunk;
		int eSquare = EditedSquares[n].square;

		std::vector<int> chunkListC;
		std::vector<int> squareListC;

		int cChunk = NorthChunk(eSquare, eChunk, 1, 1, chunkListC);
		int cSquare = NorthSquare(eSquare, eChunk, 1, 1, squareListC);

		int cDirection = 1;

		ConnectEdges(eChunk, eSquare, cChunk, cSquare, cDirection);

		/////////

		std::vector<int> chunkListC2;
		std::vector<int> squareListC2;

		cChunk = WestChunk(eSquare, eChunk, 1, 1, chunkListC2);
		cSquare = WestSquare(eSquare, eChunk, 1, 1, squareListC2);

		cDirection = 4;

		ConnectEdges(eChunk, eSquare, cChunk, cSquare, cDirection);

		/////////

		std::vector<int> chunkListC3;
		std::vector<int> squareListC3;

		cChunk = EastChunk(eSquare, eChunk, 1, 1, chunkListC3);
		cSquare = EastSquare(eSquare, eChunk, 1, 1, squareListC3);

		cDirection = 2;

		ConnectEdges(eChunk, eSquare, cChunk, cSquare, cDirection);

		///////// (it seems this last one isn't necessary)

		std::vector<int> chunkListC4;
		std::vector<int> squareListC4;

		cChunk = SouthChunk(eSquare, eChunk, 1, 1, chunkListC4);
		cSquare = SouthSquare(eSquare, eChunk, 1, 1, squareListC4);

		cDirection = 3;

		ConnectEdges(eChunk, eSquare, cChunk, cSquare, cDirection);

	}

	while (!EditedSquares.empty())
	{
		EHM = EditedSquares.back().square;
		EHC = EditedSquares.back().chunk;

		//ConnectEdges(chunkNum, squareNum, newChunkNum, newSquareNum, 1);



		UpdateTerrain(EHC, EHM, updateTextures, Graphics_Device);

		EditedSquares.pop_back();
	}



}




VOID MapList::UpdateTerrain(int chunkIndex, int TerrainNum, bool updateTextures, LPDIRECT3DDEVICE9 Graphics_Device)
{



	


	//start Terrain Updating loop

	TerrainMeshData *TMDD = new TerrainMeshData(Graphics_Device);

	int modifiedDetailLevel = 1;
	modifiedDetailLevel = (int)pow(2.0, (TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].detailLevel - 1));

	//create drawing mesh
	newMesh = TMDD->GetTerrainMesh(this, chunkIndex, TerrainNum, modifiedDetailLevel, true);
	if (TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Mesh != NULL)
	{
		TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Mesh->Release();
	}
	if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Mesh))) {
	}

	TMDD->Release();


	//create target mesh
	newMesh = TMDD->GetTerrainMesh(this, chunkIndex, TerrainNum, 1, false);//modifiedDetailLevel
	if (TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Target_Terrain_Mesh != NULL)
	{
		TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Target_Terrain_Mesh->Release();
	}
	if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Target_Terrain_Mesh))) {
	}

	TMDD->Release();



	////////////////////////////////////////////////////////
	if (updateTextures && modifiedDetailLevel != 0)
	{

		for (int bm = 0; bm < 8; bm++)
		{

			//create drawing mesh
			newMesh = TMDD->GetTerrainBlendingMesh(this, chunkIndex, TerrainNum, 1, bm);
			if (TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Blending_Mesh[bm] != NULL)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Blending_Mesh[bm]->Release();
			}
			if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[chunkIndex].TerrainMapData[TerrainNum].Terrain_Blending_Mesh[bm]))) {
			}

			TMDD->Release();

		}


	}
	////////////////////////////////////////////////////////
	delete TMDD;


	//end Terrain Updating loop


}







//called from NewSquare if the chunk number passed to NewSquare doesn't exist
VOID MapList::NewChunk(int chunkIndex)
{




}

//NOTE: add another function for adding edges between two squares (click one square, then the other)

//temporary function for creating a new square
//note: squareIndex is the one you click on, not the one that is about to be created
VOID MapList::NewSquare(int squareLength, int squareWidth, int squareIndex, int side, int chunkIndex, LPDIRECT3DDEVICE9 Graphics_Device)
{

	//side: 1=north 2=east 3=south 4=west

	//create a new chunk if it doesn't exist
	bool chunkExists = false;
	for (int cI = 0; cI < TerrainMapChunks.size(); cI++)
	{
		if (TerrainMapChunks[cI].index == chunkIndex)
		{
			chunkExists = true;
		}
	}
	if (chunkExists == false)
	{
		NewChunk(chunkIndex);
	}


	MapData newMapData;

	newMapData.length = squareLength;
	newMapData.width = squareWidth;

	newMapData.position = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].position;

	newMapData.visible = false;


	if (side == 1)
	{
		newMapData.position.z = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].position.z + TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].length;
	}
	if (side == 2)
	{
		newMapData.position.x = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].position.x + TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].width;
	}
	if (side == 3)
	{
		newMapData.position.z = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].position.z - TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].length;
	}
	if (side == 4)
	{
		newMapData.position.x = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].position.x - TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].width;
	}
	newMapData.squareNum = TerrainMapChunks[chunkIndex].TerrainMapData.size();

	//initialize to null
	newMapData.Terrain_Mesh = NULL;
	newMapData.Target_Terrain_Mesh = NULL;
	for (int bm = 0; bm < 8; bm++)
	{
		newMapData.Terrain_Blending_Mesh[bm] = NULL;
	}



	TerrainMapChunks[chunkIndex].TerrainMapData.push_back(newMapData);

	int newIndex = TerrainMapChunks[chunkIndex].TerrainMapData.size() - 1;

	int mapIndex = newIndex;
	int WriteMapNumX = 0;
	int WriteMapNumY = 0;


	for (WriteMapNumX = 0; WriteMapNumX <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; WriteMapNumX++)
	{
		for (WriteMapNumY = 0; WriteMapNumY <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; WriteMapNumY++)
		{

			//sets the new square's y values to be equal to those of the edge from which it was spawned
			if (side == 1)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY] = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].heightMap[WriteMapNumX][TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length - 1];
			}
			if (side == 2)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY] = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].heightMap[TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width - 1][WriteMapNumY];
			}
			if (side == 3)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY] = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].heightMap[WriteMapNumX][0];
			}
			if (side == 4)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY] = TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].heightMap[0][WriteMapNumY];
			}




			TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[WriteMapNumX][WriteMapNumY] = 1;
		}
	}

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Terrain_Mesh = NULL;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Target_Terrain_Mesh = NULL;
	for (int bm = 0; bm < 8; bm++)
	{
		TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Terrain_Blending_Mesh[bm] = NULL;
	}




















	//if the mesh is not visible, create it in order to prevent an error
	if (TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].visible == false)
	{
		//at this point we are simply creating the mesh at detail level 2, really this could be any detail level we want
		if (TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Terrain_Mesh == NULL || TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].detailLevel != 2)
		{
			TerrainMeshData *TMDD2 = new TerrainMeshData(Graphics_Device);

			newMesh = TMDD2->GetTerrainMesh(this, chunkIndex, newIndex, 1, true);
			if (TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Terrain_Mesh != NULL)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Terrain_Mesh->Release();
			}
			if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Terrain_Mesh))) {
			}
			TMDD2->Release();
			//create target mesh
			newMesh = TMDD2->GetTerrainMesh(this, chunkIndex, newIndex, 1, false);
			if (TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Target_Terrain_Mesh != NULL)
			{
				TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Target_Terrain_Mesh->Release();
			}
			if (FAILED(newMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].Target_Terrain_Mesh))) {
			}
			/*
			for(int bm = 0; bm < 8; bm ++)
			{

			//create drawing mesh
			newMesh = TMDD2->GetTerrainBlendingMesh(this, EHM, 1, bm);
			if(TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm] != NULL)
			{
			TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm]->Release();
			}
			if (FAILED(newMesh->CloneMeshFVF(0,D3DFVF_IMPROVEDVERTEX,Graphics_Device,&TerrainMapChunks[chunkIndex].TerrainMapData[EHM].Terrain_Blending_Mesh[bm]))) {
			}

			TMDD2->Release();

			}
			*/
			TMDD2->Release();
			delete TMDD2;

			TerrainMapChunks[chunkIndex].TerrainMapData[newIndex].detailLevel = 1;
		}
	}



}

//connect two squares by adding an EdgeData object to each of them, representing the other
VOID MapList::ConnectSquares(int Chunk1, int Square1, int Chunk2, int Square2)
{


	int xDir = TerrainMapChunks[Chunk1].TerrainMapData[Square1].position.x - TerrainMapChunks[Chunk2].TerrainMapData[Square2].position.x;
	int zDir = TerrainMapChunks[Chunk1].TerrainMapData[Square1].position.z - TerrainMapChunks[Chunk2].TerrainMapData[Square2].position.z;


	int side = 0;

	if (xDir > 0)
	{
		side = 2;
	}
	if (xDir < 0)
	{
		side = 4;
	}
	if (zDir > 0)
	{
		side = 1;
	}
	if (zDir < 0)
	{
		side = 3;
	}

	//for now, assume all squares have the same dimentions
	//this will change soon

	if (side != 0)
	{

		MapEdgeData mapEdge;

		//create edge data to go in square 1

		mapEdge.chunkNum = Chunk2;
		mapEdge.squareNum = Square2;

		mapEdge.leftIndex = 0;
		mapEdge.rightIndex = TerrainMapChunks[Chunk1].TerrainMapData[Square1].width;

		if (side == 1)
		{
			mapEdge.direction = 's';
		}
		if (side == 2)
		{
			mapEdge.direction = 'w';
		}
		if (side == 3)
		{
			mapEdge.direction = 'n';
		}
		if (side == 4)
		{
			mapEdge.direction = 'e';
		}

		TerrainMapChunks[Chunk1].TerrainMapData[Square1].edges.push_back(mapEdge);


		//create edge data to go in square 2

		mapEdge.chunkNum = Chunk1;
		mapEdge.squareNum = Square1;

		mapEdge.leftIndex = 0;
		mapEdge.rightIndex = TerrainMapChunks[Chunk2].TerrainMapData[Square2].width;

		if (side == 1)
		{
			mapEdge.direction = 'n';
		}
		if (side == 2)
		{
			mapEdge.direction = 'e';
		}
		if (side == 3)
		{
			mapEdge.direction = 's';
		}
		if (side == 4)
		{
			mapEdge.direction = 'w';
		}

		TerrainMapChunks[Chunk2].TerrainMapData[Square2].edges.push_back(mapEdge);



	}

	//reset active square variables to reset the tool for another use
	activeSquare = 0;
	activeChunk = 0;

}

//temporary function for creating a new map
VOID MapList::NewMap()
{



}

int MapList::FindModelIndex(int ind)
{
	for (int i = 0; i < ModelList.size(); i++)
	{
		if (ind == ModelList[i].index)
		{
			return i;
		}
	}
	return 0;//did not find
}

//reads in the chunk list from the world map file
VOID  MapList::LoadWorldMap(string MapFolderString)
{
	std::stringstream ss;
	string mapString;


	//helpFile1 << mapString;
	//helpFile1 << "\n";

	//for the square itself
	int numSquares, squareIndex, length, width, position;

	//for each edge
	int connectSquare, left, right;
	char direction;
	string mapFileString = "..\\Maps\\" + MapFolderString + "\\WorldMap.txt";
	char mapFileChar[255];
	strcpy_s(mapFileChar, mapFileString.c_str());
	fstream mapFile(mapFileChar);
	
	//get number of squares
	getline(mapFile, mapString, ';');


	ss.clear();
	ss << mapString;
	ss >> numSquares;
	ss.clear();

	numChunks = numSquares;

	for (int n = 0; n < numChunks; n++)
	{
		//helpFile1 << "\n";
		MapChunk mapChunk;


		//get filename
		getline(mapFile, mapString, ' ');

		mapChunk.filename = mapString;

		//get square index
		getline(mapFile, mapString, '(');

		ss.clear();
		ss << mapString;
		ss >> squareIndex;
		ss.clear();

		mapChunk.index = squareIndex;

		//get position
		//getline(mapFile, mapString, '(');
		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapChunk.position.x = position;

		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapChunk.position.y = position;

		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapChunk.position.z = position;

		getline(mapFile, mapString, ')');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapChunk.radius = position;

		//get width and length
		getline(mapFile, mapString, '{');

		getline(mapFile, mapString, '}');

		ss.clear();
		ss << mapString;
		ss >> numSquares;
		ss.clear();

		mapChunk.numSquares = numSquares;

		getline(mapFile, mapString, ';');


		TerrainMapChunks.push_back(mapChunk);

	}


}

//reads in a single terrain chunk (the list of all the mapdata in that chunk)
VOID  MapList::LoadMapChunk(int chunkIndex, string MapFolderString)
{
	//Map1 1 (0,0,0) {10,10} [1-2,e,0,10] [1-11,n,0,10];[

	std::stringstream ss;
	string mapString;
	string mapIndexString, mapFileNameString;


	int numSquares, squareIndex, length, width, position;

	//for each edge
	int connectSquare, left, right;
	char direction;
	ss.clear();
	ss << chunkIndex;
	ss >> mapIndexString;
	ss.clear();
	string mapFileString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + mapIndexString + ".txt";
	char mapFileChar[255];
	strcpy_s(mapFileChar, mapFileString.c_str());
	fstream mapFile(mapFileChar);


	//get number of squares
	getline(mapFile, mapString, ';');


	ss.clear();
	ss << mapString;
	ss >> numSquares;
	ss.clear();

	//duplicate from world map, but keep just in case
	TerrainMapChunks[chunkIndex].numSquares = numSquares;


	MapData mapData0;


	mapData0.filename = "Map0";
	mapData0.squareNum = 0;
	mapData0.position.x = 0;
	mapData0.position.y = 0;
	mapData0.position.z = 0;
	mapData0.length = 10;
	mapData0.width = 10;


	TerrainMapChunks[chunkIndex].TerrainMapData.push_back(mapData0);
	/*
	TerrainMapChunks[chunkIndex].TerrainMapData[0].squareNum = 0;
	TerrainMapChunks[chunkIndex].TerrainMapData[0].visible = false;
	TerrainMapChunks[chunkIndex].TerrainMapData[0].detailLevel = 2;
	*/
	ReadSingleHeightMap(chunkIndex, 0, MapFolderString);


	for (int n = 1; n <= TerrainMapChunks[chunkIndex].numSquares; n++)
	{
		//helpFile1 << "\n";
		MapData mapData;


		//get filename
		getline(mapFile, mapString, ' ');

		mapData.filename = mapString;

		//get square index
		getline(mapFile, mapString, '(');

		ss.clear();
		ss << mapString;
		ss >> squareIndex;
		ss.clear();

		mapData.squareNum = squareIndex;

		//get position
		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapData.position.x = position;



		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapData.position.y = position;

		getline(mapFile, mapString, ')');

		ss.clear();
		ss << mapString;
		ss >> position;
		ss.clear();

		mapData.position.z = position;

		if (chunkIndex == 2)
		{
			//mapData.position.x+=160;
			//mapData.position.z+=640;
		}


		//get width and length
		getline(mapFile, mapString, '{');

		getline(mapFile, mapString, ',');

		ss.clear();
		ss << mapString;
		ss >> numSquares;
		ss.clear();

		mapData.length = numSquares;

		getline(mapFile, mapString, '}');

		ss.clear();
		ss << mapString;
		ss >> numSquares;
		ss.clear();

		mapData.width = numSquares;



		//get edge data

		int failCounter = 0;
		//get connecting edges
		while (failCounter <= 50)
		{
			failCounter++;
			MapEdgeData mapEdge;


			getline(mapFile, mapString, '[');

			if (mapString == ";")
			{
				int shjashd = 0;
				break;
			}
			getline(mapFile, mapString, '-');

			ss.clear();
			ss << mapString;
			ss >> connectSquare;
			ss.clear();

			mapEdge.chunkNum = connectSquare;

			getline(mapFile, mapString, ',');

			ss.clear();
			ss << mapString;
			ss >> connectSquare;
			ss.clear();

			mapEdge.squareNum = connectSquare;

			getline(mapFile, mapString, ',');

			ss.clear();
			ss << mapString;
			ss >> direction;
			ss.clear();

			mapEdge.direction = direction;

			getline(mapFile, mapString, ',');

			ss.clear();
			ss << mapString;
			ss >> left;
			ss.clear();

			mapEdge.leftIndex = left;

			getline(mapFile, mapString, ']');

			ss.clear();
			ss << mapString;
			ss >> right;
			ss.clear();

			mapEdge.rightIndex = right;


			//TerrainMapChunks[chunkIndex].TerrainMapData[squareIndex].edges.push_back(mapEdge);
			mapData.edges.push_back(mapEdge);

		}






		//getline(mapFile, mapString, ';');


		TerrainMapChunks[chunkIndex].TerrainMapData.push_back(mapData);

		ReadSingleHeightMap(chunkIndex, n, MapFolderString);
		ReadSingleTextureMap(chunkIndex, n, MapFolderString);

		ReadSingleStaticObjectData(chunkIndex, n, MapFolderString);



	}



	int jdhfsdkj = 0;
}

//reads in a single terrain square's heightmap
VOID  MapList::ReadSingleHeightMap(int chunkIndex, int mapIndex, string MapFolderString)
{

	//Read the data from the map file and save it to heightMap

	std::stringstream ss;
	fstream mapFiles;

	string string1;
	/*
	MapData m1;
	m1.squareNum = 1;

	TerrainMapChunks[chunkIndex].TerrainMapData.push_back(m1);
	*/
	string mapIndexString, mapChunkString, mapFileNameString;

	float min = 0;
	float max = 0;

	//InitMapData(1);

	MapData tempMapData;

	float HMtemp = 0;
	//mapFileNameString = "..\\Maps\\"+MapFolderString+"\\Chunk1\\Map"+mapIndexString+".txt";

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].distanceToCamera = 0;
	//-----Open Map File-----
	ss.clear();
	ss << mapIndex;
	ss >> mapIndexString;
	ss.clear();
	ss.clear();
	ss << chunkIndex;
	ss >> mapChunkString;
	ss.clear();
	mapFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + mapChunkString + "\\Map" + mapIndexString + ".txt";
	char mapFileName[255];
	strcpy_s(mapFileName, mapFileNameString.c_str());
	mapFiles.open(mapFileName);
	//-----

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].visible = NULL;

	for (int WriteMapNumX = 0; WriteMapNumX <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; WriteMapNumX++)
	{
		for (int WriteMapNumY = 0; WriteMapNumY <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; WriteMapNumY++)
		{
			getline(mapFiles, string1, ',');

			ss.clear();
			ss << string1;
			ss >> HMtemp;//heightMap[mapIndex][WriteMapNumX][WriteMapNumY];
			ss.clear();
			//heightMap[mapIndex][WriteMapNumX][WriteMapNumY] = HMtemp;
			if (HMtemp < min)
			{
				min = HMtemp;
			}
			if (HMtemp > max)
			{
				max = HMtemp;
			}
			TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY] = HMtemp;//heightMap[mapIndex][WriteMapNumX][WriteMapNumY];
		}
	}

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].visible = false;

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Terrain_Mesh = NULL;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Target_Terrain_Mesh = NULL;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].temp_Terrain_Mesh = NULL;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].temp_Target_Terrain_Mesh = NULL;
	for (int bm = 0; bm < 8; bm++)
	{
		TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].Terrain_Blending_Mesh[bm] = NULL;
		TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].temp_Terrain_Blending_Mesh[bm] = NULL;
	}
	mapFiles.close();

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin = TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax = TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position;

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.x *= 2;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.z *= 2;

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.x *= 2;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.z *= 2;

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.x += TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width * 2;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.z += TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length * 2;

	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMin.y = min * 2;
	TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].BBMax.y = max * 2;





}

//reads in a single terrain square's texture map
VOID  MapList::ReadSingleTextureMap(int chunkIndex, int mapIndex, string MapFolderString)
{

	//Read the data from the map file and save it to heightMap

	std::stringstream ss;
	fstream mapFiles;

	string string1;
	/*
	MapData m1;
	m1.squareNum = 1;

	TerrainMapChunks[chunkIndex].TerrainMapData.push_back(m1);
	*/
	string mapIndexString, mapChunkString, mapFileNameString;


	//InitMapData(1);

	MapData tempMapData;

	float HMtemp = 1;

	//-----Open Map File-----
	ss.clear();
	ss << mapIndex;
	ss >> mapIndexString;
	ss.clear();
	ss.clear();
	ss << chunkIndex;
	ss >> mapChunkString;
	ss.clear();
	mapFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + mapChunkString + "\\TextureMap" + mapIndexString + ".txt";
	char mapFileName[255];
	strcpy_s(mapFileName, mapFileNameString.c_str());
	mapFiles.open(mapFileName);
	//-----

	for (int WriteMapNumX = 0; WriteMapNumX < TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; WriteMapNumX++)
	{
		for (int WriteMapNumY = 0; WriteMapNumY < TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; WriteMapNumY++)
		{
			getline(mapFiles, string1, '_');

			ss.clear();
			ss << string1;
			ss >> HMtemp;
			ss.clear();

			TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].depthMap[WriteMapNumX][WriteMapNumY] = HMtemp;

			getline(mapFiles, string1, '_');

			ss.clear();
			ss << string1;
			ss >> HMtemp;
			ss.clear();

			TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].materialMap[WriteMapNumX][WriteMapNumY] = HMtemp;


			getline(mapFiles, string1, ',');

			ss.clear();
			ss << string1;
			ss >> HMtemp;
			ss.clear();

			TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[WriteMapNumX][WriteMapNumY] = HMtemp;
		}
	}





}

//reads in a single terrain square's static object data
VOID  MapList::ReadSingleStaticObjectData(int chunkIndex, int objectIndex, string MapFolderString)
{


	//Read the data from the object files

	std::stringstream ss;
	fstream objectFiles;

	string string1;



	int numObjects = 0;

	string objectName;



	string objectIndexString, mapChunkString, objectFileNameString;


	//mapList.TerrainMapChunks[chunkIndex].TerrainMapData
	MapData tempSceneryObject;
	tempSceneryObject.detailLevel = 1;


	//-----Open Map File-----
	ss.clear();
	ss << objectIndex;
	ss >> objectIndexString;
	ss.clear();
	ss.clear();
	ss << chunkIndex;
	ss >> mapChunkString;
	ss.clear();
	objectFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + mapChunkString + "\\Objects" + objectIndexString + ".txt";
	char objectFileName[255];
	strcpy_s(objectFileName, objectFileNameString.c_str());
	objectFiles.open(objectFileName);

	numObjects = 0;

	getline(objectFiles, string1, ';');

	ss.clear();
	ss << string1;
	ss >> numObjects;
	ss.clear();

	tempSceneryObject.numObjects = numObjects;


	for (int i = 1; i <= numObjects; i++)
	{

		getline(objectFiles, string1, ',');

		ss.clear();
		ss << string1;
		ss >> objectName;
		ss.clear();

		int modelIndex = 0;
		int modelVectorIndex = 0;
		bool modelFound = false;
		int ObjectDataVectorIndex = 0;


		//find index
		for (int mL = 0; mL < ModelList.size(); mL++)
		{
			if (ModelList[mL].name == objectName)
			{
				modelIndex = ModelList[mL].index;
				modelVectorIndex = mL;
			}

		}

		//check if ObjectData exists for the current model
		for (int oM = 0; oM < tempSceneryObject.objectMeshes.size(); oM++)
		{
			if (tempSceneryObject.objectMeshes[oM].index == modelIndex)
			{
				modelFound = true;
				ObjectDataVectorIndex = oM;
			}

		}

		//ObjectData not found, so we create one
		if (modelFound == false)
		{
			ObjectDataVectorIndex = tempSceneryObject.objectMeshes.size();
			ObjectData tempObjectData;
			tempObjectData.index = modelIndex;
			tempObjectData.objectMesh1 = ModelList[modelVectorIndex].ObjectMesh[0];
			tempSceneryObject.objectMeshes.push_back(tempObjectData);

		}

		//now we can begin filling in the instance information
		ObjectInstance tempObjectInstance;

		//get position data
		getline(objectFiles, string1, '(');

		//x
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.position.x;
		ss.clear();

		//y
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.position.y;
		ss.clear();

		//z
		getline(objectFiles, string1, ')');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.position.z;
		ss.clear();



		//get scale data
		getline(objectFiles, string1, '{');

		//x
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.scale.x;
		ss.clear();

		//y
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.scale.y;
		ss.clear();

		//z
		getline(objectFiles, string1, '}');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.scale.z;
		ss.clear();


		//get rotation data
		getline(objectFiles, string1, '[');

		//x
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.rotation.x;
		ss.clear();

		//y
		getline(objectFiles, string1, ',');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.rotation.y;
		ss.clear();

		//z
		getline(objectFiles, string1, ']');
		ss.clear();
		ss << string1;
		ss >> tempObjectInstance.rotation.z;
		ss.clear();

		//Note: change the following lines to accept negative numbers (add widths/heights instead of subtracting)
		//that way, we don't need the -1000 offset and we can start the map at 0,0,0 and go negative if we want

		//set up heightMap offset data
		tempObjectInstance.heightMapX = (int)(tempObjectInstance.position.x) / 2;
		if (tempObjectInstance.heightMapX > 0)
		{
			while (tempObjectInstance.heightMapX > TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].width)
			{
				tempObjectInstance.heightMapX -= TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].width;
			}
		}

		if (tempObjectInstance.heightMapX < 0)
		{
			while (tempObjectInstance.heightMapX < 0)// TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].width)
			{
				tempObjectInstance.heightMapX += TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].width;
			}
		}

		tempObjectInstance.heightMapZ = (int)(tempObjectInstance.position.z) / 2;
		if (tempObjectInstance.heightMapZ > 0)
		{
			while (tempObjectInstance.heightMapZ > TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].length)
			{
				tempObjectInstance.heightMapZ -= TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].length;
			}
		}

		if (tempObjectInstance.heightMapZ > 0)
		{
			while (tempObjectInstance.heightMapZ < 0)//TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].length)
			{
				tempObjectInstance.heightMapZ += TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].length;
			}
		}

		tempObjectInstance.heightMapY = TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].heightMap[tempObjectInstance.heightMapX][tempObjectInstance.heightMapZ];


		tempSceneryObject.objectMeshes[ObjectDataVectorIndex].instances.push_back(tempObjectInstance);

	}


	if (TerrainMapChunks[chunkIndex].TerrainMapData.size() > objectIndex)
	{
		TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes = tempSceneryObject.objectMeshes;
		TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].numObjects = tempSceneryObject.numObjects;
	}

	//mapList.TerrainMapChunks[chunkIndex].TerrainMapData.push_back(tempSceneryObject);

	objectFiles.close();

}


//initialize the list of objects
//note: may want to change this later to load up a list based on what chunks are visible
//specifically, we would want to take out the create mesh function, and put that somewhere within the chunk init loop
//but for now, in the editor, we need to have every object available, and improvements to efficiency can wait a bit
VOID MapList::InitStaticObjectList(string MapFolderString, LPDIRECT3DDEVICE9 Graphics_Device)
{

	std::stringstream ss;
	string modelString;
	string objectFilenameString;

	LPD3DXBUFFER pD3DXMtrlBuffer;


	//helpFile1 << mapString;
	//helpFile1 << "\n";

	//for the square itself
	int numObjects, modelIndex, detailLevels;

	//for each edge
	int connectSquare, left, right;
	char direction;
	string modelFileString = "..\\Maps\\" + MapFolderString + "\\ObjectList1.txt";
	char modelFileChar[255];
	strcpy_s(modelFileChar, modelFileString.c_str());
	fstream modelFile(modelFileChar);

	//get number of objects
	getline(modelFile, modelString, ';');


	ss.clear();
	ss << modelString;
	ss >> numObjects;
	ss.clear();


	for (int n = 0; n < numObjects; n++)
	{
		//helpFile1 << "\n";
		ObjectModel newObject;


		//get model index
		getline(modelFile, modelString, ',');


		ss.clear();
		ss << modelString;
		ss >> newObject.index;
		ss.clear();

		//get model name
		getline(modelFile, modelString, ',');

		ss.clear();
		ss << modelString;
		ss >> newObject.name;
		ss.clear();

		//get number of detail levels
		getline(modelFile, modelString, ',');

		ss.clear();
		ss << modelString;
		ss >> detailLevels;
		ss.clear();

		for (int d = 0; d < detailLevels; d++)
		{

			//get filenames
			getline(modelFile, modelString, ',');

			ss.clear();
			ss << modelString;
			ss >> objectFilenameString;
			ss.clear();

			char objectFileName[255];
			strcpy_s(objectFileName, objectFilenameString.c_str());

			// Load the mesh from the specified file
			if (FAILED(D3DXLoadMeshFromX(objectFileName, D3DXMESH_SYSTEMMEM,
				Graphics_Device, NULL,
				&pD3DXMtrlBuffer, NULL, &newObject.ObjectNumMaterials[d],
				&newObject.ObjectMesh[d])))
			{
				objectFilenameString = "..\\Models\\" + objectFilenameString;
				strcpy_s(objectFileName, objectFilenameString.c_str());
				// If model is not in current folder, try parent folder
				if (FAILED(D3DXLoadMeshFromX(objectFileName, D3DXMESH_SYSTEMMEM,
					Graphics_Device, NULL,
					&pD3DXMtrlBuffer, NULL, &newObject.ObjectNumMaterials[d],
					&newObject.ObjectMesh[d])))
				{

					//return E_FAIL;
				}
			}

			// We need to extract the material properties and texture names from the 
			// pD3DXMtrlBuffer
			D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
			newObject.ObjectMeshMaterials[d] = new D3DMATERIAL9[newObject.ObjectNumMaterials[d]];
			if (newObject.ObjectNumMaterials[d] == NULL)
			{
				//return E_OUTOFMEMORY;
			}
			newObject.ObjectMeshTextures[d] = new LPDIRECT3DTEXTURE9[newObject.ObjectNumMaterials[d]];
			if (newObject.ObjectMeshTextures[d] == NULL)
			{
				//return E_OUTOFMEMORY;
			}

			for (DWORD i = 0; i<newObject.ObjectNumMaterials[d]; i++)
			{
				// Copy the material
				newObject.ObjectMeshMaterials[d][i] = d3dxMaterials[i].MatD3D;

				// Set the ambient color for the material (D3DX does not do this)
				newObject.ObjectMeshMaterials[d][i].Ambient = newObject.ObjectMeshMaterials[d][i].Diffuse;
				/*
				////////////
				//probably temporary (until I can properly export materials in Blender
				//also, note that decreasing the .a values makes it transparent
				//this could be done in the draw loop temporarily for meshes that are
				//intersected by a ray from the camera to the player
				newObject.ObjectMeshMaterials[d][i].Ambient.a =1;
				newObject.ObjectMeshMaterials[d][i].Ambient.r =.2;
				newObject.ObjectMeshMaterials[d][i].Ambient.g =.2;
				newObject.ObjectMeshMaterials[d][i].Ambient.b =.2;

				newObject.ObjectMeshMaterials[d][i].Diffuse.a =1;
				newObject.ObjectMeshMaterials[d][i].Diffuse.r =.2;
				newObject.ObjectMeshMaterials[d][i].Diffuse.g =.2;
				newObject.ObjectMeshMaterials[d][i].Diffuse.b =.2;

				newObject.ObjectMeshMaterials[d][i].Specular.a =1;
				newObject.ObjectMeshMaterials[d][i].Specular.r =.1;
				newObject.ObjectMeshMaterials[d][i].Specular.g =.1;
				newObject.ObjectMeshMaterials[d][i].Specular.b =.1;

				newObject.ObjectMeshMaterials[d][i].Emissive.a =1;
				newObject.ObjectMeshMaterials[d][i].Emissive.r =.0;
				newObject.ObjectMeshMaterials[d][i].Emissive.g =.0;
				newObject.ObjectMeshMaterials[d][i].Emissive.b =.0;

				newObject.ObjectMeshMaterials[d][i].Power = 1.5;
				////////////
				*/
				newObject.ObjectMeshTextures[d][i] = NULL;
				if (d3dxMaterials[i].pTextureFilename != NULL &&
					lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
				{
					// Create the texture
					if (FAILED(D3DXCreateTextureFromFileA(Graphics_Device,
						d3dxMaterials[i].pTextureFilename,
						&newObject.ObjectMeshTextures[d][i])))
					{
						// If texture is not in current folder, try parent folder
						const CHAR* strPrefix = "..\\Models\\";
						CHAR strTexture[MAX_PATH];
						StringCchCopyA(strTexture, MAX_PATH, strPrefix);
						StringCchCatA(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
						// If texture is not in current folder, try parent folder
						if (FAILED(D3DXCreateTextureFromFileA(Graphics_Device,
							strTexture,
							&newObject.ObjectMeshTextures[d][i])))
						{

						}
					}
				}
			}

			//DWORD adjac[101*101*6];
			//newObject.ObjectMesh[d]->GenerateAdjacency(0.1f, adjac);
			//D3DXComputeNormals(newObject.ObjectMesh[d], adjac);

		}


		ModelList.push_back(newObject);



	}



}



VOID MapList::InitTerrainTextureList(string MapFolderString, LPDIRECT3DDEVICE9 Graphics_Device)
{

	std::stringstream ss;
	string texturesString;
	string textureFilenameString;


	//for the square itself
	int numTextures, modelIndex, detailLevels, materialNum, heightNum;

	//for each edge
	int connectSquare, left, right;
	char direction;
	string textureFileString = "..\\Maps\\" + MapFolderString + "\\TextureList1.txt";
	char textureFileChar[255];
	strcpy_s(textureFileChar, textureFileString.c_str());
	fstream texturelFile(textureFileChar);

	//get number of objects
	getline(texturelFile, texturesString, ';');


	ss.clear();
	ss << texturesString;
	ss >> numTextures;
	ss.clear();


	for (int n = 0; n < numTextures; n++)
	{
		//helpFile1 << "\n";
		TextureData newTexture;
		//mapList.TerrainTextureData


		//get model index
		getline(texturelFile, texturesString, ',');


		ss.clear();
		ss << texturesString;
		ss >> newTexture.index;
		ss.clear();

		//get model name
		getline(texturelFile, texturesString, ',');

		ss.clear();
		ss << texturesString;
		ss >> newTexture.name;
		ss.clear();

		//get material number
		getline(texturelFile, texturesString, ',');

		ss.clear();
		ss << texturesString;
		ss >> materialNum;
		ss.clear();

		//set material

		newTexture.terrainMaterial = SetupMaterial(0, Graphics_Device);

		//get height number (for determining blending in/out)
		getline(texturelFile, texturesString, ',');

		ss.clear();
		ss << texturesString;
		ss >> heightNum;
		ss.clear();

		//set material

		newTexture.height = heightNum;

		//get number of detail levels
		getline(texturelFile, texturesString, ',');

		ss.clear();
		ss << texturesString;
		ss >> detailLevels;
		ss.clear();

		for (int d = 0; d < detailLevels; d++)
		{

			//get filenames
			getline(texturelFile, texturesString, ',');

			ss.clear();
			ss << texturesString;
			ss >> textureFilenameString;
			ss.clear();

			char textureFileName[255];
			strcpy_s(textureFileName, textureFilenameString.c_str());

			LPDIRECT3DTEXTURE9 tempTexture;

			//create a texture from image file
			if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &tempTexture)))
			{
				// If texture is not in current folder, try parent folder
				textureFilenameString = "..\\Textures\\" + textureFilenameString;
				strcpy_s(textureFileName, textureFilenameString.c_str());
				if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &tempTexture)))
				{
					//return E_FAIL;
				}
			}



			newTexture.terrainTexture.push_back(tempTexture);



		}


		TerrainTextureData.push_back(newTexture);



	}
}

D3DMATERIAL9 MapList::SetupMaterial(int matType, LPDIRECT3DDEVICE9 Graphics_Device)
{

	D3DMATERIAL9 mtrl;

	//create  material
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	// Set the RGBA for diffuse reflection.
	mtrl.Diffuse.r = 0.6f;
	mtrl.Diffuse.g = 0.6f;
	mtrl.Diffuse.b = 0.6f;
	mtrl.Diffuse.a = 1.0f;

	// Set the RGBA for ambient reflection.
	mtrl.Ambient.r = 0.6f;
	mtrl.Ambient.g = 0.6f;
	mtrl.Ambient.b = 0.6f;
	mtrl.Ambient.a = 1.0f;

	// Set the color and sharpness of specular highlights.
	mtrl.Specular.r = 0.025f;
	mtrl.Specular.g = 0.025f;
	mtrl.Specular.b = 0.025f;
	mtrl.Specular.a = 0.025f;
	mtrl.Power = 1.5f;

	// Set the RGBA for emissive color.
	mtrl.Emissive.r = 0.0f;
	mtrl.Emissive.g = 0.0f;
	mtrl.Emissive.b = 0.0f;
	mtrl.Emissive.a = 1.0f;

	Graphics_Device->SetMaterial(&mtrl);

	return mtrl;
}



//save the map
VOID MapList::SaveMap(string MapFolderString)
{

	string string1;

	std::stringstream ss;
	fstream mapFiles;
	int mapIndex = 1;
	string mapIndexString, chunkIndexString, mapFileNameString;


	//save the world map file



	//-----Open Map File-----
	ss.clear();
	ss << mapIndex;
	ss >> mapIndexString;
	ss.clear();
	mapFileNameString = "..\\Maps\\" + MapFolderString + "\\WorldMap.txt";
	char mapFileName[255];
	strcpy_s(mapFileName, mapFileNameString.c_str());
	mapFiles.open(mapFileName, ios::out | ios::trunc);
	//-----


	//number of chunks
	int numChunks = TerrainMapChunks.size() - 1;
	mapFiles << numChunks;
	mapFiles << ";";
	mapFiles << "\n";


	for (int chunkIndex = 1; chunkIndex < TerrainMapChunks.size(); chunkIndex++)
	{

		mapFiles << "Chunk";
		mapFiles << chunkIndex;
		mapFiles << " ";
		mapFiles << chunkIndex;
		mapFiles << " ";

		mapFiles << "(";
		mapFiles << TerrainMapChunks[chunkIndex].position.x;
		mapFiles << ",";
		mapFiles << TerrainMapChunks[chunkIndex].position.y;
		mapFiles << ",";
		mapFiles << TerrainMapChunks[chunkIndex].position.z;
		mapFiles << ",";
		mapFiles << TerrainMapChunks[chunkIndex].radius;
		mapFiles << ")";

		mapFiles << " ";
		mapFiles << "{";
		mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData.size() - 1;
		mapFiles << "}";
		mapFiles << ";";
		mapFiles << "\n";

	}


	mapFiles.close();





	//////////////////////////////////////////////

	//save chunk files
	for (int chunkIndex = 1; chunkIndex < TerrainMapChunks.size(); chunkIndex++)
	{

		//-----Open Map File-----
		ss.clear();
		ss << chunkIndex;
		ss >> chunkIndexString;
		ss.clear();
		mapFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + chunkIndexString + ".txt";
		char mapFileName[255];
		strcpy_s(mapFileName, mapFileNameString.c_str());
		mapFiles.open(mapFileName, ios::out | ios::trunc);
		//-----

		//number of terrain squares
		mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData.size() - 1;
		mapFiles << ";";
		mapFiles << "\n";

		for (mapIndex = 1; mapIndex < TerrainMapChunks[chunkIndex].TerrainMapData.size(); mapIndex++)
		{

			mapFiles << "Map";
			mapFiles << mapIndex;
			mapFiles << " ";
			mapFiles << mapIndex;
			mapFiles << " ";

			//position
			mapFiles << "(";
			mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position.x;
			mapFiles << ",";
			mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position.y;
			mapFiles << ",";
			mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].position.z;
			mapFiles << ")";
			mapFiles << " ";

			//size of square
			mapFiles << "{";
			mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width;
			mapFiles << ",";
			mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length;
			mapFiles << "}";


			//loop for edges
			for (int edgeNum = 0; edgeNum < TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges.size(); edgeNum++)
			{
				mapFiles << " ";
				mapFiles << "[";
				mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges[edgeNum].chunkNum;
				mapFiles << "-";
				mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges[edgeNum].squareNum;
				mapFiles << ",";
				mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges[edgeNum].direction;
				mapFiles << ",";
				mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges[edgeNum].leftIndex;
				mapFiles << ",";
				mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].edges[edgeNum].rightIndex;
				mapFiles << "]";
			}
			mapFiles << ";";
			mapFiles << "[";
			mapFiles << "\n";


		}

		mapFiles.close();


		for (mapIndex = 1; mapIndex < TerrainMapChunks[chunkIndex].TerrainMapData.size(); mapIndex++)
		{

			//-----Open Map File-----
			ss.clear();
			ss << mapIndex;
			ss >> mapIndexString;
			ss.clear();
			mapFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + chunkIndexString + "\\Map" + mapIndexString + ".txt";
			char mapFileName[255];
			strcpy_s(mapFileName, mapFileNameString.c_str());
			mapFiles.open(mapFileName, ios::out | ios::trunc);
			//-----

			for (int WriteMapNumX = 0; WriteMapNumX <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; WriteMapNumX++)
			{
				for (int WriteMapNumY = 0; WriteMapNumY <= TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; WriteMapNumY++)
				{
					mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].heightMap[WriteMapNumX][WriteMapNumY];
					mapFiles << ",";
				}
				mapFiles << "\n";
			}
			mapFiles.close();
		}


		for (mapIndex = 1; mapIndex < TerrainMapChunks[chunkIndex].TerrainMapData.size(); mapIndex++)
		{

			//-----Open Map File-----
			ss.clear();
			ss << mapIndex;
			ss >> mapIndexString;
			ss.clear();
			mapFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + chunkIndexString + "\\TextureMap" + mapIndexString + ".txt";
			char mapFileName[255];
			strcpy_s(mapFileName, mapFileNameString.c_str());
			mapFiles.open(mapFileName, ios::out | ios::trunc);
			//-----

			for (int WriteMapNumX = 0; WriteMapNumX < TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].width; WriteMapNumX++)
			{
				for (int WriteMapNumY = 0; WriteMapNumY < TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].length; WriteMapNumY++)
				{
					mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].depthMap[WriteMapNumX][WriteMapNumY];
					mapFiles << "_";
					mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].materialMap[WriteMapNumX][WriteMapNumY];
					mapFiles << "_";
					mapFiles << TerrainMapChunks[chunkIndex].TerrainMapData[mapIndex].textureMap[WriteMapNumX][WriteMapNumY];
					mapFiles << ",";
				}
				mapFiles << "\n";
			}
			mapFiles.close();
		}


		///////////////////////////////////////////////////////


		fstream objectFiles;
		int objectIndex = 1;
		string objectIndexString, objectFileNameString;
		//Save Static Objects
		for (objectIndex = 1; objectIndex < TerrainMapChunks[chunkIndex].TerrainMapData.size(); objectIndex++)
		{

			//-----Open Map File-----
			ss.clear();
			ss << objectIndex;
			ss >> objectIndexString;
			ss.clear();
			objectFileNameString = "..\\Maps\\" + MapFolderString + "\\Chunks\\Chunk" + chunkIndexString + "\\Objects" + objectIndexString + ".txt";
			char objectFileName[255];
			strcpy_s(objectFileName, objectFileNameString.c_str());
			objectFiles.open(objectFileName, ios::out | ios::trunc);
			//-----

			//get the correct count of numObjects
			int tempNumObjects = 0;
			for (int oM = 0; oM < TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes.size(); oM++)
			{
				for (int oI = 0; oI < TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances.size(); oI++)
				{
					tempNumObjects++;
				}
			}

			TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].numObjects = tempNumObjects;

			objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].numObjects;
			objectFiles << ";";
			objectFiles << "\n";

			for (int oM = 0; oM < TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes.size(); oM++)
			{
				string modelName;
				for (int mL = 0; mL < ModelList.size(); mL++)
				{
					if (ModelList[mL].index == TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].index)
					{
						modelName = ModelList[mL].name;
					}
				}
				for (int oI = 0; oI < TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances.size(); oI++)
				{

					objectFiles << modelName;
					objectFiles << ", (";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].position.x;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].position.y;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].position.z;
					objectFiles << ") {";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].scale.x;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].scale.y;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].scale.z;
					objectFiles << "} [";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].rotation.x;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].rotation.y;
					objectFiles << ",";
					objectFiles << TerrainMapChunks[chunkIndex].TerrainMapData[objectIndex].objectMeshes[oM].instances[oI].rotation.z;
					objectFiles << "]";
					objectFiles << "\n";

				}
			}
			objectFiles.close();


		}

		//end save Static Objects

	}//end big chunk loop


}