#ifndef EditorInput_H
#define EditorInput_H

#include "CameraData.h"
#include "MapList.h"

#include <d3d9.h>
#include <d3dx9.h>

#pragma once
class EditorInput
{
public:
	EditorInput();
	void GetInput(CameraData &cameraData, MapList &mapList, void(*ptr)(char));


	~EditorInput();
	CameraData cameraData;
	float cameraSpeed = 1.5f;
	int TerrainTool = 0;//the tool for editing terrain
	//these are used for the keys which need to change the operation only once per click
	bool keyPlaceObjectDown = false;
	bool keyChangeObjectDown = false;
	bool keyChangeObjectUp = false;
	bool keyChangeTextureDown = false;
	bool keyChangeTextureUp = false;

	float HeightMagnitude = 1.0f;
	float ToolRadius = 10.0f;
	float ToolHeight = 0.0f;
	int TextureSelected = 0;
	int MaterialSelected = 1;
	int TextureDepth = 1;


	float placeObjectHeight = 0.0f;
	float placeObjectRotation = 0.0f;
	float placeObjectSize = 1.0f;


	bool PlaceObject = false;
	bool RaiseObject = false;
	bool RotateObject = false;
	bool ResizeObject = false;
	int PlaceObjectID = 0;

	int DisplayEdgeLines = 1;

	int keyCounter = 0;


	D3DXVECTOR3 vEyePt;//(cameraData.EyePtX, cameraData.EyePtY, cameraData.EyePtZ);
	D3DXVECTOR3 vLookatPt;// (cameraData.vLookatPtX, cameraData.vLookatPtY, cameraData.vLookatPtZ);
	D3DXVECTOR3 vUpVec;// (0.0f, 1.0f, 0.0f);
};

#endif
