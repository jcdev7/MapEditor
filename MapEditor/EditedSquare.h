#ifndef EditedSquare_H
#define EditedSquare_H

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include "ConnectingEdge.h"

class EditedSquare
{
public:

	int chunk;
	int square;

	std::vector<ConnectingEdge> ConnectingEdges;


private:



};

#endif