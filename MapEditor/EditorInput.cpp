

#include "EditorInput.h"

#include <Windows.h>
#include "CameraData.h"

EditorInput::EditorInput()
{
	//set initial camera values
	cameraData.EyePtX = 0.0f;
	cameraData.EyePtY = 10.0f;
	cameraData.EyePtZ = -10.0f;
	cameraData.vLookatPtX = 0.0f;
	cameraData.vLookatPtY = 0.0f;
	cameraData.vLookatPtZ = 0.0f;
	cameraData.CenterPtX = 0.0f;
	cameraData.CenterPtY = 0.0f;
	cameraData.CenterPtZ = 0.0f;
	cameraData.vEyeRotate = 3.1415926535f*1.50000000f;
	cameraData.vDistance = 10.0f;

	cameraSpeed = 1.5f;
}


//-----------------------------------------------------------------------------
// Name: GetInput()
// Desc: handle keyboard input
//-----------------------------------------------------------------------------
void EditorInput::GetInput(CameraData &cameraData, MapList &mapList, void(*ptr)(char))//(inputData cameraData)
{




	if (GetKeyState(VK_UP) & 0x80)
	{
		cameraData.vLookatPtY -= .10f;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		cameraData.vLookatPtY += .10f;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{

	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{

	}

	//yes, this is ugly math. It'll have to be replaced eventually
	//move camera forward
	if (GetKeyState('W') & 0x80 || GetKeyState('w') & 0x80)
	{
		cameraData.CenterPtX -= cameraSpeed*cos(cameraData.vEyeRotate);
		cameraData.CenterPtZ -= cameraSpeed*sin(cameraData.vEyeRotate);

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	//move camera backward
	if (GetKeyState('S') & 0x80 || GetKeyState('s') & 0x80)
	{
		cameraData.CenterPtX += cameraSpeed*cos(cameraData.vEyeRotate);
		cameraData.CenterPtZ += cameraSpeed*sin(cameraData.vEyeRotate);

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	//move camera left
	if (GetKeyState('A') & 0x80 || GetKeyState('w') & 0x80)
	{
		cameraData.CenterPtX -= cameraSpeed*cos(cameraData.vEyeRotate + (3.1415926535f / 2));
		cameraData.CenterPtZ -= cameraSpeed*sin(cameraData.vEyeRotate + (3.1415926535f / 2));

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	//move camera right
	if (GetKeyState('D') & 0x80 || GetKeyState('s') & 0x80)
	{
		cameraData.CenterPtX += cameraSpeed*cos(cameraData.vEyeRotate + (3.1415926535f / 2));
		cameraData.CenterPtZ += cameraSpeed*sin(cameraData.vEyeRotate + (3.1415926535f / 2));

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	//zoom in
	if (GetKeyState('Q') & 0x80 || GetKeyState('w') & 0x80)
	{
		cameraData.EyePtY -= .10f;
		cameraData.vLookatPtY -= .10f;

		cameraData.vDistance -= .10f;

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));

	}
	//zoom out
	if (GetKeyState('E') & 0x80 || GetKeyState('s') & 0x80)
	{
		cameraData.EyePtY += .10f;
		cameraData.vLookatPtY += .10f;

		cameraData.vDistance += .10f;

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));

	}
	//rotate
	if (GetKeyState('R') & 0x80 || GetKeyState('s') & 0x80)
	{
		cameraData.vEyeRotate += .1250f;

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	//rotate
	if (GetKeyState('F') & 0x80 || GetKeyState('s') & 0x80)
	{
		cameraData.vEyeRotate -= .1250f;

		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	if (GetKeyState('M') & 0x80)
	{
		//temporary key binding to save map data

		(*ptr) ('M');//callback
		//////////////////////////////////////////////////////////////////////////////////////////////
		//mapList.SaveMap(MapFolderString);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}
	//terrain tool selection
	if (GetKeyState('1') & 0x80)
	{
		TerrainTool = 1;
	}
	if (GetKeyState('2') & 0x80)
	{
		TerrainTool = 2;
	}
	if (GetKeyState('3') & 0x80)
	{
		TerrainTool = 3;
	}
	if (GetKeyState('4') & 0x80)
	{
		TerrainTool = 4;
	}
	if (GetKeyState('5') & 0x80)
	{
		TerrainTool = 5;
	}
	if (GetKeyState('6') & 0x80)
	{
		if (keyPlaceObjectDown == false)
		{
			TerrainTool = 0;
			if (!PlaceObject)
			{
				PlaceObject = true;
				placeObjectHeight = 0.0f;
				placeObjectRotation = 0.0f;
				placeObjectSize = 1.0f;
			}
			else
			{
				PlaceObject = false;
			}
		}
		keyPlaceObjectDown = true;
	}
	if (!(GetKeyState('6') & 0x80))
	{
		keyPlaceObjectDown = false;
	}
	if (GetKeyState('7') & 0x80)
	{
		if (keyChangeObjectDown == false)
		{
			if (PlaceObjectID > 0)
			{
				PlaceObjectID -= 1;
			}
			else if (PlaceObjectID == 0)
			{
				PlaceObjectID = mapList.ModelList.size() - 1;
			}
			keyChangeObjectDown = true;
		}
	}
	if (!(GetKeyState('7') & 0x80))
	{
		keyChangeObjectDown = false;
	}
	if (GetKeyState('8') & 0x80)
	{
		if (keyChangeObjectUp == false)
		{
			if (PlaceObjectID < mapList.ModelList.size())
			{
				PlaceObjectID += 1;
			}
			if (PlaceObjectID == mapList.ModelList.size())
			{
				PlaceObjectID = 0;
			}
			keyChangeObjectUp = true;
		}
	}
	if (!(GetKeyState('8') & 0x80))
	{
		keyChangeObjectUp = false;
	}
	if (GetKeyState(VK_OEM_MINUS) & 0x80)
	{
		if (ToolRadius > 0)
		{
			ToolRadius -= .1;
		}
	}
	if (GetKeyState(VK_OEM_PLUS) & 0x80)
	{
		if (ToolRadius < 100)
		{
			ToolRadius += .1;
		}
	}
	if (GetKeyState(VK_OEM_4) & 0x80)// "["
	{
		ToolHeight -= .5;
	}
	if (GetKeyState(VK_OEM_6) & 0x80)// "]"
	{
		ToolHeight += .5;
	}
	if (GetKeyState(VK_OEM_5) & 0x80)// "\"
	{
		ToolHeight = 0;
	}
	if (GetKeyState('9') & 0x80)
	{
		HeightMagnitude -= .1;
	}
	if (GetKeyState('0') & 0x80)
	{
		HeightMagnitude += .1;
	}
	if (GetKeyState('P') & 0x80)//tool for add new terrain square
	{
		TerrainTool = 9;
	}
	if (GetKeyState('O') & 0x80)
	{
		//also initialize these two values to prevent any possible errors:
		mapList.activeChunk = 0;
		mapList.activeSquare = 0;

		TerrainTool = 10;
	}
	if (GetKeyState('K') & 0x80)//turn edge lines on
	{
		DisplayEdgeLines = 0;
	}
	if (GetKeyState('L') & 0x80)//turn edge lines off
	{
		DisplayEdgeLines = 1;
	}

	RaiseObject = false;
	if (GetKeyState(VK_SHIFT) & 0x80)
	{
		RaiseObject = true;
	}

	RotateObject = false;
	if (GetKeyState(VK_CONTROL) & 0x80)
	{
		RotateObject = true;
	}

	ResizeObject = false;
	if (GetKeyState(VK_MENU) & 0x80)//Alt
	{
		ResizeObject = true;
	}
	if (GetKeyState('T') & 0x80)//paint textures
	{
		TerrainTool = 6;
	}
	if (GetKeyState('G') & 0x80)//blend textures
	{
		TerrainTool = 7;
	}
	if (GetKeyState('Y') & 0x80)
	{
		if (keyChangeTextureDown == false)
		{
			if (keyCounter == 0)
			{
				keyCounter = 1;
				if (TextureSelected > 0)
				{
					TextureSelected -= 1;
				}
				else if (TextureSelected == 0)
				{
					TextureSelected = mapList.TerrainTextureData.size() - 1;
				}
			}
			keyChangeTextureDown = true;
		}
	}
	if (!(GetKeyState('Y') & 0x80))
	{
		keyChangeTextureDown = false;
	}
	if (GetKeyState('U') & 0x80)
	{
		if (keyChangeTextureUp == false)
		{
			if (keyCounter == 0)
			{
				keyCounter = -1;
				if (TextureSelected < mapList.TerrainTextureData.size())
				{
					TextureSelected += 1;
				}
				if (TextureSelected == mapList.TerrainTextureData.size())
				{
					TextureSelected = 0;
				}
			}
			keyChangeTextureUp = true;
		}
	}
	if (GetKeyState('U') & 0x80)
	{
		keyChangeTextureUp = false;
	}
	if (!(GetKeyState('U') & 0x80) && !(GetKeyState('Y') & 0x80))
	{
		keyCounter = 0;
	}

}


EditorInput::~EditorInput()
{
}
