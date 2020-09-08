#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <string>



#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library


using namespace std;

//This class contains the data associated with the pool of threads for terrain updating
class UpdateThread
{
public:

	HANDLE hThread;
	unsigned threadID;

	//square data used to identify the thread and tell if it needs to be updated again if a new thread with the same square comes in
	int chunk;
	int square;
	int detailLevel;
	bool NMatched;
	bool SMatched;
	bool EMatched;
	bool WMatched;

	bool running;
	bool finished;
	bool readyToReplace;


	//these hold the waiting square's data for when the current square is still updating
	int waitingChunk;
	int waitingSquare;
	int waitingDetailLevel;


private:

protected:

};
