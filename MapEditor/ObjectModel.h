#ifndef ObjectModel_H
#define ObjectModel_H


#pragma once


#include <d3d9.h>
#include <d3dx9.h>


#include <vector>
#include <string>

using namespace std;

class ObjectModel
{
public:
	int index;//the index
	string name;//the name of the model
	LPD3DXMESH ObjectMesh[4];//the actual mesh of the model (with 0,1,2,3 for detail levels)
	DWORD ObjectNumMaterials[4];   // Number of mesh materials
	D3DMATERIAL9* ObjectMeshMaterials[40]; // Materials for our mesh
	LPDIRECT3DTEXTURE9* ObjectMeshTextures[40]; // Textures for our mesh


private:


protected:

};

#endif