#ifndef ObjectData_H
#define ObjectData_H

class ObjectInstance;      //forward declaration


#pragma once


#include <d3d9.h>
#include <d3dx9.h>


#include <vector>

#include "ObjectInstance.h"



class ObjectData
{
public:
	int index;//the numerical index specifying which object to use
	ID3DXMesh* objectMesh1;//a pointer to the mesh, would have multiple meshes (objectMesh2, etc) for high and low detail
						   //also, this mesh will be in a vector of models (a struct containing each resolution of mesh for the given object)
						   //the mesh itself is stored separately from the SceneryObjects vector in order to conserve memory by ensuring that the actual mesh is only 
						   //ever stored in 1 location, and there is not a separate copy for each terrain square
						   //this model vector will update itself to only hold meshes that are currently in use
						   //(you search the model vector by searching for the .index value, not the vector's [index])



	std::vector<ObjectInstance> instances;


private:


protected:

};

#endif