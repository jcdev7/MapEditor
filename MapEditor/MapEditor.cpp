
#include <Windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <time.h>
#include <ctime>

#include <math.h>
#include <cmath>


#include <process.h>

#include "TerrainMeshData.h"
#include "MapData.h"
#include "MapEdgeData.h"
#include "MapList.h"
#include "MapChunk.h"

#include "TextureData.h"

#include "ObjectModel.h"
#include "ObjectData.h"
#include "ObjectInstance.h"

#include "UpdateThread.h"

#include "Interface.h"
#include "InterfaceController.h"

#include "CameraData.h"
#include "EditorInput.h"

#include <sstream>
#include <strsafe.h>
#include <vector>
#include <cmath>

//for file writing/reading
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

#pragma comment (lib, "d3d9.lib")  // D3D library
#pragma comment (lib, "d3dx9.lib") // D3DX library



//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------


VOID DisplayFPS(float, int, int, int);

VOID BuildViewFrustum();


VOID GetInput();

//----------------------------------------------------------------------------
// Structs
//----------------------------------------------------------------------------


#define D3DFVF_IMPROVEDVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//the vertex type
struct IMPROVEDVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	FLOAT tu, tv;   //texture coordinates
	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
	};
};

//------------------------------

MapList mapList;

//------------------------------


struct BoundingSphere
{
	D3DXVECTOR3 position;
	float radius;
};

//----------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------

//for rendering meshes loaded from .x files
LPD3DXMESH              PlaneMesh = NULL; // Our mesh object in sysmem
LPD3DXBUFFER pD3DXMtrlBuffer;
DWORD                   g_dwNumMaterials = 0L;   // Number of mesh materials
D3DMATERIAL9*           g_pMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9*     g_pMeshTextures = NULL; // Textures for our mesh

												//for rendering the skybox loaded from .x file
LPD3DXMESH              SkyBoxMesh = NULL; // Our mesh object in sysmem
LPD3DXBUFFER SkyBoxMtrlBuffer;
DWORD                   SkyBoxNumMaterials = 0L;   // Number of mesh materials
D3DMATERIAL9*           SkyBoxMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9*     SkyBoxMeshTextures = NULL; // Textures for our mesh

Interface mapEditorInterface;

string MapFolderString = "MapFolder1";

enum Game_State { State_MainMenu, State_Game, State_Quit };
Game_State GameState = State_MainMenu;//defines whether the game is in the game itself or the menus, or paused, etc

//for bug reporting
fstream helpFile1("..\\Help1.txt");


int polygonCounter = 0;


float placeObjectX = 0.0f;
float placeObjectY = 0.0f;
float placeObjectZ = 0.0f;


static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bLastMouseLeft;//true if mouse was down, false if else
static bool bMouseRight;
static bool bMouseLeft;
static bool bMouseWheel;


float numtime = 0.0f;


int nearDistance = 0;
int farDistance = 0;
int currentDetailLevel = 1;

//adjustable detail settings (what distances to use which detail levels)
int detailSettings[8] = { 0, 25000, 50000, 100000, 150000, 400000 };
//int detailSettings[8] = {0, 1, 500, 700, 900, 1000};

float FogDensity = 0.00002f;

int WriteMapNumX = 0;
int WriteMapNumY = 0;


int MapWidth = 50;
int MapLength = 50;



int initialScreenWidth = 1600;
int initialScreenHeight = 900;

float ScreenWidth = 1680;
float ScreenHeight = 1050;


float heightMap[10001][101][101];
int textureMap[10001][100][100];

int MapSize = 0;//the number of squares in the world map


float lx = 0;//temp var for moving light
float ly = 0;//temp var for moving light
float lz = 0;//temp var for moving light


int counter = 0;


clock_t frameBeginC, frameEndC;
double frameTimer = 0;

MOUSEMOVEPOINT MouseLocation;


LPDIRECT3D9             g_pD3D = NULL; //creates the graphics device
LPDIRECT3DDEVICE9       Graphics_Device = NULL; //the rendering device

LPDIRECT3DTEXTURE9      Object_Texture = NULL;
LPDIRECT3DTEXTURE9      Object_Texture2 = NULL;
LPDIRECT3DTEXTURE9      Menu_Texture = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

D3DVIEWPORT9 viewport;

LPDIRECT3DVERTEXBUFFER9 NewCursor_Buffer = NULL;


ID3DXMesh *Terrain_Mesh[10001];//optimized mesh for drawing
ID3DXMesh *Target_Terrain_Mesh[10001];//unoptimized mesh for picking
ID3DXMesh *Terrain_Bounding_Mesh[10001];
BoundingSphere Terrain_Bounding_Sphere[10001];

LPD3DXMESH        Cursor_Mesh = NULL;
D3DMATERIAL9 Cursor_Material;

ID3DXMesh *newMesh;

CameraData cameraData;

D3DXVECTOR3 vEyePt(cameraData.EyePtX, cameraData.EyePtY, cameraData.EyePtZ);
D3DXVECTOR3 vLookatPt(cameraData.vLookatPtX, cameraData.vLookatPtY, cameraData.vLookatPtZ);
D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

D3DXMATRIXA16 matView;
D3DXMATRIXA16 matProj;
D3DXMATRIXA16 matWorld;


D3DXPLANE viewFrustum[6];


std::vector<UpdateThread> updateThreads;


int num = 0;

int num1 = 0;
int num2 = 0;


EditorInput* editorInput = new EditorInput();


D3DMATERIAL9 mtrl;

//font object
LPD3DXFONT m_font;


//note: we are using the EditedSquare class just for the square num and chunk num, any other lookup is done in MapList
std::vector<EditedSquare> VisibleTerrainList;//this is the vector telling us which squares are visible

std::vector<EditedSquare> ModifiedTerrainList;//this is the vector telling us which squares need to be loaded (a subset of VisibleTerrainList)



											  //-----------------------------------------------------------------------------
											  // Name: InitD3D()
											  // Desc: Initializes Direct3D
											  //-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	//create d3d object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;



	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//------------------------------------------------------------------------
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//------------------------------------------------------------------------

	//remove the 60 fps cap
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the D3DDevice
	//if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &Graphics_Device)))
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &Graphics_Device)))
	{
		return E_FAIL;
	}

	Graphics_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// turn on culling (don't render back facing (counter clockwise) triangles)
	//Graphics_Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );


	// Turn off D3D lighting
	// Graphics_Device->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Turn on the zbuffer
	Graphics_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	//Sets up the fog

	DWORD FogColor = D3DCOLOR_ARGB(100, 100, 100, 100);
	DWORD FogMode = D3DFOG_EXP2;
	FogDensity = 0.00025f;


	Graphics_Device->SetRenderState(D3DRS_FOGENABLE, TRUE);

	Graphics_Device->SetRenderState(D3DRS_FOGCOLOR, FogColor);


	Graphics_Device->SetRenderState(D3DRS_FOGTABLEMODE, FogMode);
	Graphics_Device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&FogDensity));







	D3DXCreateFont(Graphics_Device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);







	return S_OK;
}






HRESULT InitCursor()
{


	//set up the material


	//create  material
	ZeroMemory(&Cursor_Material, sizeof(D3DMATERIAL9));

	// Set the RGBA for diffuse reflection.
	Cursor_Material.Diffuse.r = 0.25f;
	Cursor_Material.Diffuse.g = 0.25f;
	Cursor_Material.Diffuse.b = 0.25f;
	Cursor_Material.Diffuse.a = 0.25f;

	// Set the RGBA for ambient reflection.
	Cursor_Material.Ambient.r = 0.07f;
	Cursor_Material.Ambient.g = 0.09f;
	Cursor_Material.Ambient.b = 0.07f;
	Cursor_Material.Ambient.a = 0.07f;

	// Set the color and sharpness of specular highlights.
	Cursor_Material.Specular.r = 0.025f;
	Cursor_Material.Specular.g = 0.025f;
	Cursor_Material.Specular.b = 0.025f;
	Cursor_Material.Specular.a = 0.025f;
	Cursor_Material.Power = 1.5f;

	// Set the RGBA for emissive color.
	Cursor_Material.Emissive.r = 0.0f;
	Cursor_Material.Emissive.g = 0.0f;
	Cursor_Material.Emissive.b = 0.0f;
	Cursor_Material.Emissive.a = 0.0f;




	//set up for the mesh

	D3DXVECTOR3 tempNormal_1;
	D3DXVECTOR3 tempNormal_2;

	D3DXVECTOR3 tempVertex_1;
	D3DXVECTOR3 tempVertex_2;
	D3DXVECTOR3 tempVertex_3;


	float tempHeight = 10.0;
	float tempHeight2 = 10.0;

	// Create the vertex buffer.
	if (FAILED(Graphics_Device->CreateVertexBuffer((MapWidth*MapLength * 6) * sizeof(IMPROVEDVERTEX),
		0, D3DFVF_IMPROVEDVERTEX,
		D3DPOOL_DEFAULT, &NewCursor_Buffer, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0

	IMPROVEDVERTEX* Cursor;
	if (FAILED(NewCursor_Buffer->Lock(0, 0, (void**)&Cursor, 0)))
		return E_FAIL;
	num = 0;

	num1 = 0;
	num2 = 0;

	tempHeight = 1.0;



	//x,y,z    y is up/down

	//Prepare for 1st triangle of quad
	tempVertex_1 = D3DXVECTOR3(-1.0f, tempHeight, -1.0f);
	tempVertex_2 = D3DXVECTOR3(1.0f, tempHeight, -1.0f);
	tempVertex_3 = D3DXVECTOR3(1.0f, tempHeight, 1.0f);

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_3, &tempVertex_1);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_2, &tempVertex_1);

	Cursor[num + 0].position = tempVertex_1;
	D3DXVec3Cross(&Cursor[num + 0].normal, &tempNormal_1, &tempNormal_2);
	Cursor[num + 0].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
	Cursor[num + 0].tu = 0;
	Cursor[num + 0].tv = 0;

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_1, &tempVertex_2);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_3, &tempVertex_2);

	Cursor[num + 1].position = tempVertex_2;
	D3DXVec3Cross(&Cursor[num + 1].normal, &tempNormal_1, &tempNormal_2);
	Cursor[num + 1].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
	Cursor[num + 1].tu = 1;
	Cursor[num + 1].tv = 0;

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_2, &tempVertex_3);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_1, &tempVertex_3);

	Cursor[num + 2].position = tempVertex_3;
	D3DXVec3Cross(&Cursor[num + 2].normal, &tempNormal_1, &tempNormal_2);
	Cursor[num + 2].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
	Cursor[num + 2].tu = 1;
	Cursor[num + 2].tv = 1;


	//Prepare for 2nd triangle of quad
	tempVertex_1 = D3DXVECTOR3(-1.0f, tempHeight, -1.0f);
	tempVertex_2 = D3DXVECTOR3(1.0f, tempHeight, 1.0f);
	tempVertex_3 = D3DXVECTOR3(-1.0f, tempHeight, 1.0f);

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_3, &tempVertex_1);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_2, &tempVertex_1);

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_1, &tempVertex_2);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_3, &tempVertex_2);

	D3DXVec3Subtract(&tempNormal_1, &tempVertex_2, &tempVertex_3);
	D3DXVec3Subtract(&tempNormal_2, &tempVertex_1, &tempVertex_3);

	Cursor[num + 3].position = tempVertex_3;
	D3DXVec3Cross(&Cursor[num + 3].normal, &tempNormal_1, &tempNormal_2);
	Cursor[num + 3].color = D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0);
	Cursor[num + 3].tu = 0;
	Cursor[num + 3].tv = 1;





	NewCursor_Buffer->Unlock();



	HRESULT hr2 = 0;

	hr2 = D3DXCreateMeshFVF((MapWidth*MapLength * 6),
		(MapWidth*MapLength * 6),
		D3DXMESH_MANAGED,
		IMPROVEDVERTEX::FVF_Flags, // This is how each vertex is laid out. 
		Graphics_Device,
		&Cursor_Mesh);



	IMPROVEDVERTEX* cmVertices = NULL;

	Cursor_Mesh->LockVertexBuffer(0, (void**)&cmVertices);
	{
		for (counter = 0; counter<(10); counter++)
		{
			cmVertices[counter] = Cursor[counter];
		}

	}
	Cursor_Mesh->UnlockVertexBuffer();


	WORD* cpIndices = NULL;

	Cursor_Mesh->LockIndexBuffer(0, (void**)&cpIndices);
	{
		for (counter = 0; counter<(MapWidth*MapLength * 6); counter += 6)
		{
			cpIndices[counter + 0] = 0 + (counter / 6 * 4);
			cpIndices[counter + 1] = 1 + (counter / 6 * 4);
			cpIndices[counter + 2] = 2 + (counter / 6 * 4);
			cpIndices[counter + 3] = 0 + (counter / 6 * 4);
			cpIndices[counter + 4] = 2 + (counter / 6 * 4);
			cpIndices[counter + 5] = 3 + (counter / 6 * 4);
		}

	}
	Cursor_Mesh->UnlockIndexBuffer();



	DWORD* cpAttributes = NULL;
	Cursor_Mesh->LockAttributeBuffer(0, &cpAttributes);
	{

		for (counter = 0; counter<(MapWidth*MapLength * 6); counter++)
		{
			cpAttributes[counter] = 0;
		}

	}
	Cursor_Mesh->UnlockAttributeBuffer();


	return S_OK;

}











//-----------------------------------------------------------------------------
// Name: InitTextures()
// Desc: Create the Textures and vertex buffers
//-----------------------------------------------------------------------------
HRESULT InitTextures()
{



	// load textures needed for meshes

	//create a texture from image file
	if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("grass2.bmp"), &Object_Texture)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("..\\grass2.bmp"), &Object_Texture)))
		{
			MessageBox(NULL, TEXT("Could not find grass1.bmp"), TEXT("Textures.exe"), MB_OK);
			return E_FAIL;
		}
	}

	//create a texture from image file
	if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("grass2low.bmp"), &Object_Texture2)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("..\\grass2low.bmp"), &Object_Texture2)))
		{
			MessageBox(NULL, TEXT("Could not find grass1.bmp"), TEXT("Textures.exe"), MB_OK);
			return E_FAIL;
		}
	}



	//create a texture from image file
	if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("Metal2.bmp"), &Menu_Texture)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, TEXT("..\\Metal2.bmp"), &Menu_Texture)))
		{
			MessageBox(NULL, TEXT("Could not find Metal2.bmp"), TEXT("Textures.exe"), MB_OK);
			return E_FAIL;
		}
	}



	//set initial camera values
	cameraData.EyePtX = 110.0f;
	cameraData.EyePtY = 10.0f;
	cameraData.EyePtZ = -10.0f;
	cameraData.vLookatPtX = 110.0f;
	cameraData.vLookatPtY = 0.0f;
	cameraData.vLookatPtZ = 0.0f;
	cameraData.CenterPtX = 110.0f;
	cameraData.CenterPtY = 0.0f;
	cameraData.CenterPtZ = 0.0f;
	cameraData.vEyeRotate = 3.1415926535f*1.50000000f;
	cameraData.vDistance = 10.0f;


	return S_OK;

}










int lastM = 0;
int lastX = 0;
int lastY = 0;
float lastDistance = 0;





VOID Cleanup()
{
	if (Graphics_Device != NULL)
		Graphics_Device->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}



//sets up  world, view, projection matrices.
VOID SetupMatrices()
{



	//get input for camera movement
	GetInput();




	// set view matrix
	D3DXVECTOR3 vEyePt(cameraData.EyePtX, cameraData.EyePtY, cameraData.EyePtZ);
	D3DXVECTOR3 vLookatPt(cameraData.vLookatPtX, cameraData.vLookatPtY, cameraData.vLookatPtZ);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXVECTOR3 vCenterPt(cameraData.CenterPtX, cameraData.CenterPtY, cameraData.CenterPtZ);

	D3DXVECTOR3 vLookatPtb(cameraData.vLookatPtX, cameraData.vLookatPtY, cameraData.vLookatPtZ);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 8, ScreenWidth / ScreenHeight, 1.0f, 10000.0f);

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	BuildViewFrustum();


	Graphics_Device->SetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matView);


	Graphics_Device->SetTransform(D3DTS_PROJECTION, &matProj);


	Graphics_Device->SetTransform(D3DTS_WORLD, &matView);

}






//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the Lights and materials for the scene.
//-----------------------------------------------------------------------------
VOID SetupLights()
{



	//create a directional light
	D3DXVECTOR3 vecDir1;

	D3DLIGHT9 light1;
	ZeroMemory(&light1, sizeof(D3DLIGHT9));
	light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r = .35f;
	light1.Diffuse.g = .35f;
	light1.Diffuse.b = .35f;
	light1.Diffuse.a = 1.0f;

	light1.Ambient.r = .25f;
	light1.Ambient.g = .25f;
	light1.Ambient.b = .25f;
	light1.Ambient.a = 1.0f;

	light1.Specular.r = .001f;
	light1.Specular.g = .001f;
	light1.Specular.b = .001f;
	light1.Specular.a = 1.0f;


	vecDir1 = D3DXVECTOR3(1, -1, 2);

	D3DXVec3Normalize((D3DXVECTOR3*)&light1.Direction, &vecDir1);
	Graphics_Device->SetLight(1, &light1);
	Graphics_Device->LightEnable(1, TRUE);
	Graphics_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Graphics_Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);




	D3DXVECTOR3 vecPos;
	D3DXVECTOR3 vecDir;

	vecPos = D3DXVECTOR3(lx, ly + 1, lz);
	vecDir = D3DXVECTOR3(0, -1, 0);


	//create a spot light for the cursor
	D3DLIGHT9 CurosrLight;
	ZeroMemory(&CurosrLight, sizeof(D3DLIGHT9));
	CurosrLight.Type = D3DLIGHT_SPOT;


	CurosrLight.Ambient.r = 5.0f;
	CurosrLight.Ambient.g = 5.0f;
	CurosrLight.Ambient.b = 5.0f;
	CurosrLight.Ambient.a = 0.0f;




	CurosrLight.Position = vecPos;
	CurosrLight.Direction = vecDir;


	CurosrLight.Attenuation0 = 0;
	CurosrLight.Falloff = 0;
	CurosrLight.Phi = 20;
	CurosrLight.Theta = 20;
	CurosrLight.Range = editorInput->ToolRadius * 2;
	Graphics_Device->SetLight(0, &CurosrLight);
	Graphics_Device->LightEnable(0, TRUE);
	Graphics_Device->SetRenderState(D3DRS_LIGHTING, TRUE);


	//turn on ambient light.
	Graphics_Device->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}












DWORD pfaceindex = 0;



struct MENUVERTEX
{
	FLOAT x, y, z, rhw; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
	FLOAT tu, tv;
};

#define D3DFVF_MENUVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)//(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

MENUVERTEX MENUvertices[] =
{
	{ 0.0f,   (ScreenHeight / 1.39), 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 0.0f, 01.0f }, // x, y, z, rhw, color
	{ ScreenWidth, ScreenHeight, 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 1.0f, 0.0f },
	{ 0.0f, ScreenHeight, 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 0.0f, 0.0f },

	{ 0.0f,  (ScreenHeight / 1.39), 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 0.0f, 1.0f }, // x, y, z, rhw, color
	{ ScreenWidth, (ScreenHeight / 1.39), 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 1.0f, 1.0f },
	{ ScreenWidth, ScreenHeight, 0.0f, 1.0f, D3DCOLOR_COLORVALUE(1.0, 1.0, 1.0, 1.0), 1.0f, 0.0f },
};



//
HRESULT InitMenu()
{

	if (FAILED(Graphics_Device->CreateVertexBuffer(6 * sizeof(MENUVERTEX),
		0, D3DFVF_MENUVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;


	MENUVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(MENUvertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, MENUvertices, sizeof(MENUvertices));

	g_pVB->Unlock();

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DisplayFPS()
// Desc: draws text showing current FPS and other information (will be replaced by a better interface later)
//-----------------------------------------------------------------------------
VOID DisplayFPS(float fps, int faceIndex, int faceX, int faceY)
{


	string faceIndexString, faceXString, faceYString;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	//create a rectangle to indicate where on the screen it should be drawn
	RECT rct;
	rct.left = 2;
	rct.right = ScreenWidth - 2;
	rct.top = 10;
	rct.bottom = rct.top + 20;

	string fpsMessage, fpsString;
	string toolString, radiusString, magnitudeString, heightString, modelIndexString, polygonCounterString;

	string currentSquareString, nextSquareString;

	std::stringstream ss;

	ss.clear();
	ss << fps;
	ss >> fpsString;
	ss.clear();

	ss.clear();
	ss << faceIndex;
	ss >> faceIndexString;
	ss.clear();
	if (faceIndex == 1337)
	{
		faceIndexString = " ";
	}

	ss.clear();
	ss << faceX;
	ss >> faceXString;
	ss.clear();

	ss.clear();
	ss << faceY;
	ss >> faceYString;
	ss.clear();

	ss.clear();
	ss << mapList.ModelList[editorInput->PlaceObjectID].index;
	ss >> modelIndexString;
	ss.clear();

	ss.clear();
	ss << editorInput->ToolRadius;
	ss >> radiusString;
	ss.clear();

	ss.clear();
	ss << editorInput->HeightMagnitude;
	ss >> magnitudeString;
	ss.clear();

	ss.clear();
	ss << editorInput->ToolHeight;
	ss >> heightString;
	ss.clear();

	if (editorInput->TerrainTool < 1 || editorInput->TerrainTool > 4)
	{
		toolString = "No Tool Selected";
	}
	if (editorInput->TerrainTool == 1)
	{
		toolString = "Raise/Lower Terrain (Mountains)";
	}
	if (editorInput->TerrainTool == 2)
	{
		toolString = "Raise/Lower Terrain (Hills)";
	}
	if (editorInput->TerrainTool == 3)
	{
		toolString = "Level Terrain at " + heightString + " meters";
	}
	if (editorInput->TerrainTool == 4)
	{
		toolString = "Smooth Terrain";
	}
	if (editorInput->TerrainTool == 5)
	{
		toolString = "Delete Objects";
	}
	if (editorInput->TerrainTool == 6)
	{
		string tempS1 = "  ";
		toolString = "Texture Terrain " + editorInput->TextureSelected + tempS1 + mapList.TerrainTextureData[editorInput->TextureSelected].name + "  ";
	}
	if (editorInput->TerrainTool == 7)
	{
		toolString = "Blend Textures";
	}
	if (editorInput->TerrainTool == 9)
	{
		toolString = "Add Terrain Square";
	}
	if (editorInput->TerrainTool == 10)
	{
		ss.clear();
		ss << mapList.activeSquare;
		ss >> currentSquareString;
		ss.clear();

		toolString = "Connect Edges: Square1:" + currentSquareString + " ";
	}
	fpsMessage = "FPS: " + fpsString + " | " + "index: " + faceIndexString + " | " + "x: " + faceXString + " | " + "y: " + faceYString + " | " + toolString + "| Radius = " + radiusString + " Magnitude = " + magnitudeString;
	if (editorInput->PlaceObject)
	{
		fpsMessage = "FPS: " + fpsString + " | " + "Place Object: " + mapList.ModelList[editorInput->PlaceObjectID].name + " Model Index: " + modelIndexString;
	}

	ss.clear();
	ss << polygonCounter;
	ss >> polygonCounterString;
	ss.clear();

	fpsMessage = fpsMessage + " Triangle Count: " + polygonCounterString;

	ss.clear();
	ss << VisibleTerrainList.size();
	ss >> polygonCounterString;
	ss.clear();

	fpsMessage = fpsMessage + " visible squares: " + polygonCounterString;

	int activeThreadCounter = 0;
	for (int i = 0; i < updateThreads.size(); i++)
	{
		if (updateThreads[i].finished == false)
		{
			activeThreadCounter++;
		}

	}

	ss.clear();
	ss << activeThreadCounter;//ModifiedTerrainList.size();
	ss >> polygonCounterString;
	ss.clear();




	//fpsMessage = fpsMessage + " updating squares: " + polygonCounterString + "   ";


	ss.clear();
	ss << updateThreads.size();
	ss >> polygonCounterString;
	ss.clear();



	//fpsMessage = fpsMessage + " total threads: " + polygonCounterString + "   ";

	char fpsC[1255];

	strcpy_s(fpsC, fpsMessage.c_str());


	//////////////////////

	int ChunkSize = mapList.TerrainMapChunks.size() - 1;
	/*
	for (int tChunkNum = 1; tChunkNum <= ChunkSize; tChunkNum++)
	{
		int MapSize = mapList.TerrainMapChunks[tChunkNum].TerrainMapData.size() - 1;

		string stringList = "";

		for (int tMeshNum = 1; tMeshNum <= MapSize; tMeshNum++)//MapSize
		{

			if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].visible == true)
			{
				fpsMessage += "(";

				ss.clear();
				ss << tChunkNum;
				ss >> stringList;
				ss.clear();

				fpsMessage += stringList;

				fpsMessage += ",";

				ss.clear();
				ss << tMeshNum;
				ss >> stringList;
				ss.clear();

				fpsMessage += stringList;

				fpsMessage += ":";

				ss.clear();
				ss << mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel;
				ss >> stringList;
				ss.clear();

				fpsMessage += stringList;

				fpsMessage += "[";

				ss.clear();
				ss << (int)mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].BBMin.y;
				ss >> stringList;
				ss.clear();

				fpsMessage += stringList;

				fpsMessage += ",";

				ss.clear();
				ss << (int)mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].BBMax.y;
				ss >> stringList;
				ss.clear();

				fpsMessage += stringList;

				fpsMessage += "]";

				fpsMessage += ")";
			}
		}
	}
	*/
	///////////////////


	if (fpsMessage.length() < 1200)
	{
		strcpy_s(fpsC, fpsMessage.c_str());
	}

	LPCSTR fpsM;

	fpsM = fpsC;

	m_font->DrawText(NULL, fpsM, -1, &rct, 0, fontColor);



}

//-----------------------------------------------------------------------------
// Name: BuildViewFrustum()
// Desc: builds view frustum
//-----------------------------------------------------------------------------
void BuildViewFrustum()
{

	D3DXMATRIX matViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	// Left plane 
	viewFrustum[0].a = matViewProj._14 + matViewProj._11;
	viewFrustum[0].b = matViewProj._24 + matViewProj._21;
	viewFrustum[0].c = matViewProj._34 + matViewProj._31;
	viewFrustum[0].d = matViewProj._44 + matViewProj._41;

	// Right plane 
	viewFrustum[1].a = matViewProj._14 - (matViewProj._11); 
	viewFrustum[1].b = matViewProj._24 - (matViewProj._21); 
	viewFrustum[1].c = matViewProj._34 - (matViewProj._31); 
	viewFrustum[1].d = matViewProj._44 - (matViewProj._41); 
	

	// Top plane 
	viewFrustum[2].a = matViewProj._14 - (matViewProj._12); 
	viewFrustum[2].b = matViewProj._24 - (matViewProj._22); 
	viewFrustum[2].c = matViewProj._34 - (matViewProj._32); 
	viewFrustum[2].d = matViewProj._44 - (matViewProj._42); 
	
	// Bottom plane 
	viewFrustum[3].a = matViewProj._14 + matViewProj._12;
	viewFrustum[3].b = matViewProj._24 + matViewProj._22;
	viewFrustum[3].c = matViewProj._34 + matViewProj._32;
	viewFrustum[3].d = matViewProj._44 + matViewProj._42;

	// Near plane 
	viewFrustum[4].a = matViewProj._13;
	viewFrustum[4].b = matViewProj._23;
	viewFrustum[4].c = matViewProj._33;
	viewFrustum[4].d = matViewProj._43;

	// Far plane 
	viewFrustum[5].a = matViewProj._14 - matViewProj._13; 
	viewFrustum[5].b = matViewProj._24 - matViewProj._23; 
	viewFrustum[5].c = matViewProj._34 - matViewProj._33; 
	viewFrustum[5].d = matViewProj._44 - matViewProj._43; 
	
	//normalize
	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize( &viewFrustum[i], &viewFrustum[i] ); 
	}
}


//-----------------------------------------------------------------------------
// Name: BoundingSphereInFrustrum()
// Desc: check if the Bounding Sphere is in the current view frustum
//-----------------------------------------------------------------------------
BOOL BoundingSphereInFrustrum(D3DXVECTOR3 center, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&viewFrustum[i], &center) + radius < 0)
		{
			return FALSE; //not visible
		}
	}
	return TRUE;
};

//-----------------------------------------------------------------------------
// Name: BoundingBoxInFrustrum()
// Desc: check if the Axis-Aligned Bounding Box is in the current view frustum
//-----------------------------------------------------------------------------
BOOL BoundingBoxInFrustrum(const D3DXVECTOR3 &aabbMin, const D3DXVECTOR3 &aabbMax)
{
	

	bool intersect = FALSE;
	WORD result = 0;
	D3DXVECTOR3 minExtreme, maxExtreme;
	D3DXVECTOR3 viewFrustumNormal;

	for (WORD i = 0; i<6; i++)
	{
		if (viewFrustum[i].a <= 0)
		{
			minExtreme.x = aabbMin.x;
			maxExtreme.x = aabbMax.x;
		}
		else
		{
			minExtreme.x = aabbMax.x;
			maxExtreme.x = aabbMin.x;
		}

		if (viewFrustum[i].b <= 0)
		{
			minExtreme.y = aabbMin.y;
			maxExtreme.y = aabbMax.y;
		}
		else
		{
			minExtreme.y = aabbMax.y;
			maxExtreme.y = aabbMin.y;
		}

		if (viewFrustum[i].c <= 0)
		{
			minExtreme.z = aabbMin.z;
			maxExtreme.z = aabbMax.z;
		}
		else
		{
			minExtreme.z = aabbMax.z;
			maxExtreme.z = aabbMin.z;
		}

		viewFrustumNormal.x = viewFrustum[i].a;
		viewFrustumNormal.y = viewFrustum[i].b;
		viewFrustumNormal.z = viewFrustum[i].c;


		int ajfhdjksf = D3DXVec3Dot(&viewFrustumNormal, &minExtreme);

		if (D3DXPlaneDotCoord(&viewFrustum[i], &minExtreme) < 0.0f)
		{
			return FALSE;
		}
		if (D3DXVec3Dot(&viewFrustumNormal, &minExtreme) + viewFrustum[i].d < 0)
		{
			//result = 0;
			//return result;
			//return FALSE;
		}

		if (D3DXVec3Dot(&viewFrustumNormal, &maxExtreme) + viewFrustum[i].d >= 0)
		{
			intersect = TRUE;
		}
	}


	return TRUE;
};



//-----------------------------------------------------------------------------
// Name: RayIntersectsBoundingBox()
// Desc: check if the ray intersects the Axis-Aligned Bounding Box
//-----------------------------------------------------------------------------
bool RayIntersectsBoundingBox(const D3DXVECTOR3 &aabbMin, const D3DXVECTOR3 &aabbMax, const D3DXVECTOR3 &rayStart, const D3DXVECTOR3 &rayDir)
{
	float tmin = (aabbMin.x - rayStart.x) / rayDir.x;
	float tmax = (aabbMax.x - rayStart.x) / rayDir.x;

	if (tmin > tmax)
	{
		swap(tmin, tmax);
	}

	float tymin = (aabbMin.y - rayStart.y) / rayDir.y;
	float tymax = (aabbMax.y - rayStart.y) / rayDir.y;

	if (tymin > tymax)
	{
		swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (aabbMin.z - rayStart.z) / rayDir.z;
	float tzmax = (aabbMax.z - rayStart.z) / rayDir.z;

	if (tzmin > tzmax)
	{
		swap(tzmin, tzmax);
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	return true;
}




bool ModifiedTerrainListContains(EditedSquare ESTest)
{

	for (int modifiedNum = 0; modifiedNum < ModifiedTerrainList.size(); modifiedNum++)
	{
		if (ModifiedTerrainList[modifiedNum].chunk == ESTest.chunk && ModifiedTerrainList[modifiedNum].square == ESTest.square)
		{
			return true;
		}
	}
	return false;
}


int fpsCounter = 0;
float fpsTimer = -1;
float fpsAmount = 0;
clock_t beginC, endC;

int threadCounter = 0;



//-----------------------------------------------------------------------------
// Name: LoadSquaresByChunk()
// Desc: simple loading of squares at startup
//-----------------------------------------------------------------------------
int LoadSquaresByChunk(int loadC, int loadS)
{

	ID3DXMesh *newUpdateMesh;

	//UpdateThread *pUThread = reinterpret_cast<UpdateThread*>(p);


	int tChunkNum = loadC;
	int tMeshNum = loadS;

	currentDetailLevel = pow(2.0, (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel - 1));

	//mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel = dNum;

	TerrainMeshData *TMDD2 = new TerrainMeshData(Graphics_Device);

	//create drawing mesh

	beginC = clock();
	endC = clock();

	fpsTimer = difftime(endC, beginC);



	beginC = clock();
	newUpdateMesh = TMDD2->GetTerrainMesh(&mapList, tChunkNum, tMeshNum, currentDetailLevel, true);
	endC = clock();
	fpsTimer = difftime(endC, beginC);

	beginC = clock();
	if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh != NULL)
	{
		mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh->Release();
	}
	if (FAILED(newUpdateMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh)))
	{
	}
	endC = clock();
	fpsTimer = difftime(endC, beginC);

	TMDD2->Release();

	//create target mesh
	newUpdateMesh = TMDD2->GetTerrainMesh(&mapList, tChunkNum, tMeshNum, 1, false);//currentDetailLevel
	if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Target_Terrain_Mesh != NULL)
	{
		mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Target_Terrain_Mesh->Release();
	}
	if (FAILED(newUpdateMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Target_Terrain_Mesh)))
	{
	}

	TMDD2->Release();


	////////////////////////////////////////////////////////

	for (int bm = 0; bm < 8; bm++)
	{

		//create drawing mesh
		newUpdateMesh = TMDD2->GetTerrainBlendingMesh(&mapList, tChunkNum, tMeshNum, currentDetailLevel, bm);
		if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Blending_Mesh[bm] != NULL)
		{
			mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Blending_Mesh[bm]->Release();
		}
		if (FAILED(newUpdateMesh->CloneMeshFVF(0, D3DFVF_IMPROVEDVERTEX, Graphics_Device, &mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Blending_Mesh[bm])))
		{
		}

		TMDD2->Release();
	}


	////////////////////////////////////////////////////////



	delete TMDD2;


	//mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel = dNum;
	mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevelUpToDate = true;

	//pUThread->finished = true;

	return 0;
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{

	Graphics_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	if (fpsTimer == -1)
	{
		beginC = clock();

	}

	endC = clock();

	fpsTimer = difftime(endC, beginC);

	fpsCounter++;

	if (fpsTimer >= 1000)
	{
		fpsAmount = fpsCounter;
		fpsCounter = 0;
		fpsTimer = -1;

	}

	polygonCounter = 0;

	// Setup the Lights and materials
	SetupLights();

	// Setup the world, view, and projection matrices
	SetupMatrices();

	D3DXMATRIX tra;
	D3DXMATRIX scale;
	D3DXMATRIX rotate;
	D3DXMATRIX scale_and_translate;



	//note: we are using the EditedSquare class just for the square num and chunk num, any other lookup is done in MapList
	//std::vector<EditedSquare> VisibleTerrainList;//this is the vector telling us which squares are visible

	//std::vector<EditedSquare> ModifiedTerrainList;//this is the vector telling us which squares need to be loaded (a subset of VisibleTerrainList)

	VisibleTerrainList.clear();
	ModifiedTerrainList.clear();

	///////////////// 0 loop (initialise the detailLevelUpToDate)


	int ChunkSize = mapList.TerrainMapChunks.size() - 1;

	for (int tChunkNum = 1; tChunkNum <= ChunkSize; tChunkNum++)
	{

		MapSize = mapList.TerrainMapChunks[tChunkNum].TerrainMapData.size() - 1;

		for (int tMeshNum = 1; tMeshNum <= MapSize; tMeshNum++)//MapSize
		{
			mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevelUpToDate = true;
		}
	}



	//////////////This first loop is just to set the detailLevels to avoid inaccurate edges later on


	//////The problem is that squares need to be updated whenever any of their neighbors are updated to a different detail level
	//this will be a fairly significant slowdown, but it's the only way to ensure accurate edge matching



	ChunkSize = mapList.TerrainMapChunks.size() - 1;

	for (int tChunkNum = 1; tChunkNum <= ChunkSize; tChunkNum++)
	{

		MapSize = mapList.TerrainMapChunks[tChunkNum].TerrainMapData.size() - 1;

		for (int tMeshNum = 1; tMeshNum <= MapSize; tMeshNum++)//MapSize
		{

			//this is a temp fix since we're no longer multithreading things
			mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].beingUpdated = false;

			mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].visible = BoundingBoxInFrustrum(mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].BBMin, mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].BBMax);


			int cameraDistance = 2000;


			if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].visible)
			{
				//temporary until bounding box culling is implemented
				int cameraDistanceX = (cameraData.EyePtX - (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].position.x * 2 + mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].width / 2));
				int cameraDistanceY = (cameraData.EyePtY - mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].position.y);
				int cameraDistanceZ = (cameraData.EyePtZ - (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].position.z * 2 + mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].length / 2));
				cameraDistance = sqrt((float)((cameraDistanceX * cameraDistanceX)*2.5 + (cameraDistanceY * cameraDistanceY)*.5 + (cameraDistanceZ * cameraDistanceZ)*2.5));
				//note that y distance is more heavily weighted than x or z
				//this is because when the camera is facing out towards the horizon, terrain is more likely to be parallel with the cammera vector
				//and therefore less visible, also there are a lot more visible squares, making higher resolution more costly for reduced gain

				mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].distanceToCamera = cameraDistance;

				//mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevelUpToDate = true;
				for (int dNum = 1; dNum < 6; dNum++)
				{
					nearDistance = detailSettings[dNum - 1];
					farDistance = detailSettings[dNum];

					currentDetailLevel = pow(2.0, (dNum - 1));


					if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].distanceToCamera < farDistance && mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].distanceToCamera >= nearDistance)//full quality view distance 1
					{

						if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel != dNum)
						{

							mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].oldDetailLevel = mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel;
							mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel = dNum;
							mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevelUpToDate = false;



						}


					}


				}
				if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].distanceToCamera >= detailSettings[5])//do not render, and do not keep mesh in memory at all 4
				{
					mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].visible = false;

				}

			}

			//check to make sure it's still visible, if it is, then add it to the list
			if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].visible)
			{
				EditedSquare tempVisibleSquare;
				tempVisibleSquare.chunk = tChunkNum;
				tempVisibleSquare.square = tMeshNum;

				VisibleTerrainList.push_back(tempVisibleSquare);
			}



		}//end square loop

	}//end chunk loop








	if (updateThreads.size() >= 2)
	{
		int sdjkhgksdj = 0;
	}



	mapList.SetupMaterial(1, Graphics_Device);
	Graphics_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	Graphics_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Graphics_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	Graphics_Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	////////////////This loop is for drawing
	for (int visibleNum = 0; visibleNum < VisibleTerrainList.size(); visibleNum++)
	{
		int tChunkNum = VisibleTerrainList[visibleNum].chunk;
		int tMeshNum = VisibleTerrainList[visibleNum].square;

		if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh != NULL
			&& mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].beingUpdated == false)
		{

			currentDetailLevel = pow(2.0, (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel - 1));
			polygonCounter += ((16 * 16 * 2) / currentDetailLevel);


			if (mapList.TerrainTextureData.size() >= 1)//bounds check, just in case
			{
				if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh != NULL
					&&mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].beingUpdated == false)
				{
					//add an option for toggling on or off (start tn at 0 instead of 1) these lines at the edge of each square
					for (int tn = editorInput->DisplayEdgeLines; tn < mapList.TerrainTextureData.size(); tn++)
					{
						//check whether the mesh has any squares with the current texture
						bool hasTexture = false;
						for (int tI = 0; tI < mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].currentTextures.size(); tI++)
						{
							if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].currentTextures[tI] == mapList.TerrainTextureData[tn].index)
							{
								hasTexture = true;
							}
						}



						if (hasTexture == true || tn == 0)
						{
							Graphics_Device->SetTexture(0, mapList.TerrainTextureData[tn].terrainTexture[0]);
							mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh->DrawSubset(tn);
						}

					}
				}
			}
		}

	}


	////////////////This loop is for drawing (the blended edges)
	for (int visibleNum = 0; visibleNum < VisibleTerrainList.size(); visibleNum++)
	{
		int tChunkNum = VisibleTerrainList[visibleNum].chunk;
		int tMeshNum = VisibleTerrainList[visibleNum].square;

		if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh != NULL
			&& mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].beingUpdated == false)
		{

			currentDetailLevel = pow(2.0, (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].detailLevel - 1));


			if (mapList.TerrainTextureData.size() >= 1)//bounds check, just in case
			{
				if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Mesh != NULL
					&&mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].beingUpdated == false)
				{
					//add an option for toggling on or off (start tn at 0 instead of 1) these lines at the edge of each square
					for (int tn = editorInput->DisplayEdgeLines; tn < mapList.TerrainTextureData.size(); tn++)
					{

						//check whether the mesh has any squares with the current texture
						bool hasTexture = false;
						for (int tI = 0; tI < mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].currentTextures.size(); tI++)
						{
							if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].currentTextures[tI] == mapList.TerrainTextureData[tn].index)
							{
								hasTexture = true;
							}
						}

						if (hasTexture == true)
						{
							Graphics_Device->SetTexture(0, mapList.TerrainTextureData[tn].terrainTexture[0]);

							bool bmLoaded = true;
							for (int bm = 0; bm < 8; bm++)
							{
								//if (mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Blending_Mesh[bm] == NULL)//29.8%
								{
									//bmLoaded = false;
								}
							}

							if (bmLoaded)
							{
								for (int bm = 0; bm < 8; bm++)
								{
									mapList.TerrainMapChunks[tChunkNum].TerrainMapData[tMeshNum].Terrain_Blending_Mesh[bm]->DrawSubset(tn);//34.5%
								}
							}

						}


					}
				}
			}
		}

	}





	////////////////////////////////////////////////////
	///////////// set position of mouse ////////////////
	////////////////////////////////////////////////////
	D3DXVECTOR3 result;
	D3DXVECTOR3 screenPoint;
	//D3DVIEWPORT9 viewport;

	screenPoint.x = (ptCurrentMousePosit.x)*1.0f; //MouseLocation.x;
	screenPoint.y = 0.0f;
	screenPoint.z = (ptCurrentMousePosit.y)*1.0f; //MouseLocation.y;

	Graphics_Device->GetViewport(&viewport);
	Graphics_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	Graphics_Device->GetTransform(D3DTS_VIEW, &matView);
	Graphics_Device->GetTransform(D3DTS_WORLD, &matWorld);
	////////////////////////////////////////////////////
	////////////// unproject from mouse ////////////////
	////////////////////////////////////////////////////


	// get coordinates from mouse
	D3DXVECTOR3 v;
	v.x = (((2.0f * ptCurrentMousePosit.x) / ScreenWidth) - 1) / matProj._11;
	v.y = -(((2.0f * ptCurrentMousePosit.y) / ScreenHeight) - 1) / matProj._22;
	v.z = 1.0f;

	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin, rayDir;

	D3DXMatrixInverse(&m, NULL, &matView);

	// transform to 3d space
	rayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	// get inverse
	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &matWorld);

	// transform origin and direction by inverse
	D3DXVECTOR3 rayObjOrigin, rayObjDirection;

	D3DXVec3TransformCoord(&rayObjOrigin, &rayOrigin, &matInverse);
	D3DXVec3TransformNormal(&rayObjDirection, &rayDir, &matInverse);
	D3DXVec3Normalize(&rayObjDirection, &rayObjDirection);



	//
	// transform mouse coordinates from 2d to 3d space and pick the mesh it intersects with
	//
	BOOL hasHit;
	float distanceToCollision = 0;
	float pU;
	float pV;
	DWORD something = 0;
	int number_squares = 8;
	int activeSquare = 1337;
	int activeChunk = 1337;
	LPD3DXBUFFER hitInfo;

	D3DXVECTOR3 lastRayObjDirection;

	float lastDistanceToCollision = 3000;//set to max distance
	int lastIndex = 0;

	for (int visibleNum = 0; visibleNum < VisibleTerrainList.size(); visibleNum++)
	{
		int numC = VisibleTerrainList[visibleNum].chunk;
		int num = VisibleTerrainList[visibleNum].square;

		if (mapList.TerrainMapChunks[numC].TerrainMapData[num].Terrain_Mesh != NULL && mapList.TerrainMapChunks[numC].TerrainMapData[num].visible
			&& mapList.TerrainMapChunks[numC].TerrainMapData[num].beingUpdated == false)
		{
			hasHit = false;

			//first check if the ray intersects the bounding box
			if (RayIntersectsBoundingBox(mapList.TerrainMapChunks[numC].TerrainMapData[num].BBMin, mapList.TerrainMapChunks[numC].TerrainMapData[num].BBMax, rayObjOrigin, rayObjDirection))
			{
				//now check with the actual mesh to find the exact position and which vertex it collides with
				D3DXIntersect(mapList.TerrainMapChunks[numC].TerrainMapData[num].Target_Terrain_Mesh, &rayObjOrigin, &rayObjDirection, &hasHit, &pfaceindex, &pU, &pV, &distanceToCollision, &hitInfo, &something);
				if (hasHit && abs(distanceToCollision) < abs(lastDistanceToCollision))
				{
					lastIndex = pfaceindex;
					activeSquare = num;
					activeChunk = numC;
					lastDistanceToCollision = distanceToCollision;
					lastRayObjDirection = rayObjDirection;
				}
			}
		}
	}



	int tempMapNumX;
	int tempMapNumY;

	tempMapNumX = lastIndex;
	tempMapNumY = 0;

	if (activeSquare != 1337 && activeChunk != 1337)
	{
		tempMapNumX *= 1;
		while (tempMapNumX >= (mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].width * 2))
		{
			tempMapNumX -= mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].width * 2;
			tempMapNumY += 1;
		}
	}
	tempMapNumX /= 2;


	DisplayFPS(fpsAmount, activeSquare, tempMapNumX, tempMapNumY);


	float IntersectX;
	float IntersectY;
	float IntersectZ;

	IntersectX = rayObjOrigin.x + lastRayObjDirection.x * lastDistanceToCollision;
	IntersectY = rayObjOrigin.y + lastRayObjDirection.y * lastDistanceToCollision;//height
	IntersectZ = rayObjOrigin.z + lastRayObjDirection.z * lastDistanceToCollision;


	//This set of settings is for rendering non-transparent tga files (e.g. tree leaves)
	//(not good for smoke/clouds/fire effects)
	//////////
	// Set Texture stage states to allow for alpha blending
	Graphics_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	Graphics_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	Graphics_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	Graphics_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	Graphics_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Graphics_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Graphics_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	Graphics_Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	Graphics_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Graphics_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	Graphics_Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0);
	Graphics_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//////////
	int chunkIndex = activeChunk;
	if (bMouseLeft)
	{

		if (activeSquare != 1337 && activeSquare != 0)
		{
			if (bLastMouseLeft == false && (editorInput->TerrainTool == 9 || editorInput->TerrainTool == 10))//only do this operation once per click
			{
				mapList.EditHeightMap(activeSquare, activeChunk, tempMapNumX, tempMapNumY, editorInput->HeightMagnitude, editorInput->ToolRadius, editorInput->ToolHeight, editorInput->TerrainTool, editorInput->TextureSelected, editorInput->MaterialSelected, editorInput->TextureDepth, Graphics_Device);//(which mesh, x, y, magnitude, radius, level, tool to use)
			}
			if (editorInput->TerrainTool != 9 && editorInput->TerrainTool != 10)
			{
				mapList.EditHeightMap(activeSquare, activeChunk, tempMapNumX, tempMapNumY, editorInput->HeightMagnitude, editorInput->ToolRadius, editorInput->ToolHeight, editorInput->TerrainTool, editorInput->TextureSelected, editorInput->MaterialSelected, editorInput->TextureDepth, Graphics_Device);//(which mesh, x, y, magnitude, radius, level, tool to use)
			}

			//adjust values of objects affected by editing the heightmap
			for (int s2 = 0; s2 < mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.size(); s2++)
			{
				for (int s3 = 0; s3 < mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes[s2].instances.size(); s3++)
				{
					mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes[s2].instances[s3].heightMapY =
						mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].heightMap[mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes[s2].instances[s3].heightMapX][mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes[s2].instances[s3].heightMapZ];
					////////////////////////////////////
				}
			}

			//check to avoid placing more than one item at the same position
			if (editorInput->PlaceObject && bLastMouseLeft == false)
			{
				mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].numObjects += 1;

				if (mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.size() < 1)
				{
					ObjectData tempObjectData;
					tempObjectData.index = mapList.ModelList[editorInput->PlaceObjectID].index;
					tempObjectData.objectMesh1 = mapList.ModelList[editorInput->PlaceObjectID].ObjectMesh[0];
					mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.push_back(tempObjectData);
				}

				int oMIndex = 0;
				bool foundObjectModel = false;
				for (int oM = 0; oM < mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.size(); oM++)
				{
					if (mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes[oM].index == mapList.ModelList[editorInput->PlaceObjectID].index)
					{
						oMIndex = oM;
						foundObjectModel = true;
					}
				}

				if (foundObjectModel == false)//add the new model to the scenery object vector
				{
					ObjectData tempObjectData;
					tempObjectData.index = mapList.ModelList[editorInput->PlaceObjectID].index;
					tempObjectData.objectMesh1 = mapList.ModelList[editorInput->PlaceObjectID].ObjectMesh[0];
					mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.push_back(tempObjectData);
					oMIndex = mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].objectMeshes.size() - 1;
				}

				ObjectInstance tempObjectInstance;
				tempObjectInstance.position.x = IntersectX;
				tempObjectInstance.position.y = IntersectY + editorInput->placeObjectHeight - mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].heightMap[tempMapNumX][tempMapNumY];
				tempObjectInstance.position.z = IntersectZ;

				tempObjectInstance.rotation.x = 0;
				tempObjectInstance.rotation.y = editorInput->placeObjectRotation;
				tempObjectInstance.rotation.x = 0;

				tempObjectInstance.scale.x = editorInput->placeObjectSize;
				tempObjectInstance.scale.y = editorInput->placeObjectSize;
				tempObjectInstance.scale.z = editorInput->placeObjectSize;

				tempObjectInstance.heightMapX = tempMapNumX;
				tempObjectInstance.heightMapZ = tempMapNumY;

				tempObjectInstance.heightMapY = mapList.TerrainMapChunks[activeChunk].TerrainMapData[activeSquare].heightMap[tempMapNumX][tempMapNumY];


				mapList.TerrainMapChunks[chunkIndex].TerrainMapData[activeSquare].objectMeshes[oMIndex].instances.push_back(tempObjectInstance);

			}

			//delete objects
			if (editorInput->TerrainTool == 5 && bLastMouseLeft == false)
			{
				for (int mS = 0; mS < mapList.TerrainMapChunks[activeChunk].TerrainMapData.size(); mS++)
				{
					for (int oM =  mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes.size() - 1; oM >= 0; oM--)
					{
						for (int oI = mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes[oM].instances.size() - 1; oI >= 0; oI--)
						{
							D3DXVECTOR2 objectPos2 = D3DXVECTOR2(mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes[oM].instances[oI].position.x, mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes[oM].instances[oI].position.z);

							float distToObj = sqrt(pow(objectPos2.x - IntersectX, 2) + pow(objectPos2.y - IntersectZ, 2));

							if (distToObj <= (editorInput->ToolRadius * 2))
							{
								mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes[oM].instances.erase(mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes[oM].instances.begin() + oI);
								mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].numObjects -= 1;
							}

						}
						//if there are no instances left, delete the object itself
						if (mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].numObjects <= 0)
						{
							mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes.erase(mapList.TerrainMapChunks[activeChunk].TerrainMapData[mS].objectMeshes.begin() + oM);
						}

					}
				}




			}
		}


	}

	//this state makes it so objects being scaled don't have their normals scaled also
	Graphics_Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);


	float TerrainHeightOffset = 0.0f;

	for (int visibleNum = 0; visibleNum < VisibleTerrainList.size(); visibleNum++)
	{
		int numC = VisibleTerrainList[visibleNum].chunk;
		int s1 = VisibleTerrainList[visibleNum].square;

		if (mapList.TerrainMapChunks[numC].TerrainMapData[s1].visible
			&& mapList.TerrainMapChunks[numC].TerrainMapData[s1].beingUpdated == false)
		{
			for (int s2 = 0; s2 < mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes.size(); s2++)
			{
				for (int s3 = 0; s3 < mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances.size(); s3++)
				{
					TerrainHeightOffset = mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].heightMapY;
					////////////////////////////////////
					//set transform matrices
					D3DXMatrixTranslation(&tra, mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].position.x,
						mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].position.y + TerrainHeightOffset,
						mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].position.z);
					D3DXMatrixScaling(&scale, mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].scale.x,
						mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].scale.y,
						mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].scale.z);
					D3DXMatrixRotationY(&rotate, mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].instances[s3].rotation.y);
					Graphics_Device->SetTransform(D3DTS_WORLD, &rotate);
					D3DXMatrixMultiply(&tra, &rotate, &tra);
					D3DXMatrixMultiply(&scale_and_translate, &scale, &tra);
					Graphics_Device->SetTransform(D3DTS_WORLD, &scale_and_translate);
					////////////////////////////////////

					int modelIndex = mapList.FindModelIndex(mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].index);


					for (DWORD i = 0; i<mapList.ModelList[modelIndex].ObjectNumMaterials[0]; i++)
					{

						// Set the material and texture for this subset
						Graphics_Device->SetMaterial(&mapList.ModelList[modelIndex].ObjectMeshMaterials[0][i]);
						Graphics_Device->SetTexture(0, mapList.ModelList[modelIndex].ObjectMeshTextures[0][i]);

						// Draw the mesh subset
						mapList.TerrainMapChunks[numC].TerrainMapData[s1].objectMeshes[s2].objectMesh1->DrawSubset(i);
					}

				}

			}
		}
	}




	lx = IntersectX;
	ly = IntersectY;
	lz = IntersectZ;

	if (editorInput->ToolRadius < 0)
	{
		editorInput->ToolRadius = 0;
	}

	float cylinderRadiusLow = editorInput->ToolRadius * 2;
	float cylinderRadiusHigh = editorInput->ToolRadius * 2;
	//release previous mesh to free up memory
	Cursor_Mesh->Release();

	//create new mesh
	float cursorHeight = 0;
	float cursorOffset = 0;
	if (editorInput->TerrainTool == 3)
	{
		cursorHeight = (editorInput->ToolHeight - IntersectY) * 2;
		if (cursorHeight < 0)
		{
			cursorOffset = cursorHeight;
			cursorHeight = abs(cursorHeight);
		}
	}
	D3DXCreateCylinder(Graphics_Device, cylinderRadiusHigh, cylinderRadiusLow, 5 + cursorHeight, 30, 30, &Cursor_Mesh, &pD3DXMtrlBuffer);

	////////////////////////////////////
	//set transform matrices
	D3DXMatrixTranslation(&tra, IntersectX, IntersectY - .99f + cursorOffset, IntersectZ);
	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationX(&rotate, 3.1415f / 2);
	Graphics_Device->SetTransform(D3DTS_WORLD, &rotate);
	D3DXMatrixMultiply(&tra, &rotate, &tra);
	D3DXMatrixMultiply(&scale_and_translate, &scale, &tra);
	Graphics_Device->SetTransform(D3DTS_WORLD, &scale_and_translate);
	////////////////////////////////////

	if (!editorInput->PlaceObject)
	{

		Graphics_Device->SetTexture(0, Object_Texture);
		Graphics_Device->SetMaterial(&Cursor_Material);

		Cursor_Mesh->DrawSubset(0);
	}

	pD3DXMtrlBuffer->Release();

	if (!editorInput->RaiseObject && !editorInput->RotateObject && !editorInput->ResizeObject)
	{
		placeObjectX = IntersectX;
		placeObjectY = IntersectY;
		placeObjectZ = IntersectZ;
	}


	if (editorInput->PlaceObject)
	{
		//This set of settings is for rendering non-transparent tga files (e.g. tree leaves)
		//////////
		// Set Texture stage states to allow for alpha blending
		Graphics_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		Graphics_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		Graphics_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		Graphics_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		Graphics_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		Graphics_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Graphics_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Graphics_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		Graphics_Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

		Graphics_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Graphics_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		Graphics_Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0);
		Graphics_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//////////
		//draws object at cursor
		//hold shift and move mounse up/down to lock x,y and adjust z height of object for placement
		//hold ctrl and move mouse left/right to rotate object for placement
		//hold alt and move moust up-right / down-left to adjust scale
		for (DWORD i = 0; i<mapList.ModelList[editorInput->PlaceObjectID].ObjectNumMaterials[0]; i++)
		{

			////////////////////////////////////
			//set transform matrices
			D3DXMatrixTranslation(&tra, placeObjectX, placeObjectY + editorInput->placeObjectHeight, placeObjectZ);
			D3DXMatrixScaling(&scale, editorInput->placeObjectSize, editorInput->placeObjectSize, editorInput->placeObjectSize);
			D3DXMatrixRotationY(&rotate, editorInput->placeObjectRotation);
			Graphics_Device->SetTransform(D3DTS_WORLD, &rotate);
			D3DXMatrixMultiply(&tra, &rotate, &tra);
			D3DXMatrixMultiply(&scale_and_translate, &scale, &tra);
			Graphics_Device->SetTransform(D3DTS_WORLD, &scale_and_translate);
			////////////////////////////////////

			// Set the material and texture for this subset
			Graphics_Device->SetMaterial(&mapList.ModelList[editorInput->PlaceObjectID].ObjectMeshMaterials[0][i]);
			Graphics_Device->SetTexture(0, mapList.ModelList[editorInput->PlaceObjectID].ObjectMeshTextures[0][i]);

			// Draw the mesh subset
			mapList.ModelList[editorInput->PlaceObjectID].ObjectMesh[0]->DrawSubset(i);
		}
	}

	bLastMouseLeft = bMouseLeft;

}



void InputCallback(char inAction)
{

	if (inAction == 'M')
	{
		mapList.SaveMap(MapFolderString);
	}

}



//-----------------------------------------------------------------------------
// Name: GetInput()
// Desc: handle keyboard input
//-----------------------------------------------------------------------------
void GetInput()
{
	void(*ptr)(char) = &InputCallback;
	editorInput->GetInput(cameraData, mapList, ptr);

	return;

}
int tempMapNumX;
int tempMapNumY;


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: mouse input
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		case VK_F1:

			break;

		case VK_F2:

			break;

		case VK_F3:

			break;
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		float MouseWheel = ((int)wParam)*1.0f; // wheel rotation 

		MouseWheel /= 1000000;

		cameraData.EyePtY -= (MouseWheel);
		cameraData.vLookatPtY -= (MouseWheel);
		cameraData.vDistance -= (MouseWheel);
		cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
		cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

		cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
		cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
	}
	break;

	case WM_MBUTTONDOWN:
	{
		ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD(lParam);
		ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD(lParam);
		bMouseWheel = true;
	}
	break;

	case WM_MBUTTONUP:
	{
		bMouseWheel = false;
	}
	break;

	case WM_RBUTTONDOWN:
	{
		ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD(lParam);
		ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD(lParam);
		bMouseRight = true;
	}
	break;

	case WM_LBUTTONDOWN:
	{
		bMouseLeft = true;
	}
	break;

	case WM_LBUTTONUP:
	{
		bMouseLeft = false;
	}
	break;

	case WM_RBUTTONUP:
	{
		bMouseRight = false;
	}
	break;

	case WM_MOUSEMOVE:
	{
		ptCurrentMousePosit.x = LOWORD(lParam);
		ptCurrentMousePosit.y = HIWORD(lParam);


		if (bMouseWheel)
		{

			cameraData.vEyeRotate += (ptCurrentMousePosit.x - ptLastMousePosit.x)*.010f;

			cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
			cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

			cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
			cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
		}


		if (bMouseRight)
		{
			cameraData.CenterPtX -= (ptCurrentMousePosit.x - ptLastMousePosit.x)*.10f*cos(cameraData.vEyeRotate + (3.1415926535f / 2));
			cameraData.CenterPtZ -= (ptCurrentMousePosit.x - ptLastMousePosit.x)*.10f*sin(cameraData.vEyeRotate + (3.1415926535f / 2));

			cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
			cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

			cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
			cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));





			cameraData.CenterPtX -= (ptCurrentMousePosit.y - ptLastMousePosit.y)*.10f*cos(cameraData.vEyeRotate);
			cameraData.CenterPtZ -= (ptCurrentMousePosit.y - ptLastMousePosit.y)*.10f*sin(cameraData.vEyeRotate);

			cameraData.EyePtX = cameraData.CenterPtX + (cameraData.vDistance*cos(cameraData.vEyeRotate));
			cameraData.EyePtZ = cameraData.CenterPtZ + (cameraData.vDistance*sin(cameraData.vEyeRotate));

			cameraData.vLookatPtX = cameraData.CenterPtX + ((10 - cameraData.vDistance)*cos(cameraData.vEyeRotate + (3.1415926535f)));
			cameraData.vLookatPtZ = cameraData.CenterPtZ + ((10 - cameraData.vDistance)*sin(cameraData.vEyeRotate + (3.1415926535f)));
		}

		if (bMouseLeft)
		{

		}

		if (editorInput->RaiseObject)
		{
			editorInput->placeObjectHeight -= .25*(ptCurrentMousePosit.y - ptLastMousePosit.y);
		}

		if (editorInput->RotateObject)
		{
			editorInput->placeObjectRotation -= (ptCurrentMousePosit.x - ptLastMousePosit.x) / 100.0f;
		}

		if (editorInput->ResizeObject)
		{
			editorInput->placeObjectSize -= ((ptCurrentMousePosit.y - ptLastMousePosit.y) - (ptCurrentMousePosit.x - ptLastMousePosit.x)) / 10.0f;
		}

		ptLastMousePosit.x = ptCurrentMousePosit.x;
		ptLastMousePosit.y = ptCurrentMousePosit.y;
	}
	break;

	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: LoadMap()
// Desc: load the terrain from files
//-----------------------------------------------------------------------------
bool LoadMap(string mapFolderString, LPDIRECT3DDEVICE9 graphics_Device)
{
	//init chunk zero
	MapChunk newChunk;
	mapList.TerrainMapChunks.push_back(newChunk);

	mapList.InitStaticObjectList(mapFolderString, graphics_Device);

	mapList.LoadWorldMap(mapFolderString);

	//prepare to load chunks
	for (int cN = 0; cN < mapList.TerrainMapChunks.size(); cN++)
	{
		mapList.LoadMapChunk(cN, mapFolderString);
		for (int sN = 0; sN < mapList.TerrainMapChunks[cN].TerrainMapData.size(); sN++)
		{
			if (cN > 0 && sN > 0)
			{
				mapList.TerrainMapChunks[cN].TerrainMapData[sN].detailLevel = 1;
				mapList.TerrainMapChunks[cN].TerrainMapData[sN].beingUpdated = false;
				//LoadSquaresByChunk cannot be in this loop since it relies on having everything already loaded
			}
		}
	}
	mapList.InitTerrainTextureList(mapFolderString, graphics_Device);

	//load chunks
	for (int cN = 0; cN < mapList.TerrainMapChunks.size(); cN++)
	{
		for (int sN = 0; sN < mapList.TerrainMapChunks[cN].TerrainMapData.size(); sN++)
		{
			mapList.TerrainMapChunks[cN].TerrainMapData[sN].detailLevel = 1;
			mapList.TerrainMapChunks[cN].TerrainMapData[sN].beingUpdated = false;
			LoadSquaresByChunk(cN, sN);
		}
	}

	return true;
}
//

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		TEXT(" "), NULL };
	RegisterClassEx(&wc);

	//get the current screen resolution
	//ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//ScreenHeight = GetSystemMetrics(SM_CYSCREEN);


	ScreenWidth = initialScreenWidth;
	ScreenHeight = initialScreenHeight;


	

	//fullscreen:   WS_EX_TOPMOST | WS_POPUP
	//windowed:    WS_OVERLAPPEDWINDOW
	// Create the application's window
	HWND hWnd = CreateWindow(TEXT(" "), TEXT("Map Editor"),
		WS_OVERLAPPEDWINDOW, 0, 0, ScreenWidth, ScreenHeight,
		NULL, NULL, wc.hInstance, NULL);

	//the CreateWindow method above creates a window where the height includes the title bar height and borders
	//adjust window so the size of the actual active area is the desired dimensions
	if (IsWindow(hWnd))
	{
		DWORD windowsStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
		DWORD extendedWindowsStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
		HMENU windowsMenu = GetMenu(hWnd);
		bool hasMenu = windowsMenu;

		RECT windowRect = { 0, 0, ScreenWidth, ScreenHeight };

		AdjustWindowRectEx(&windowRect, windowsStyle, hasMenu, extendedWindowsStyle);

		SetWindowPos(hWnd, NULL, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOZORDER | SWP_NOMOVE);
		
	}


	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the scene geometry
		if (SUCCEEDED(InitTextures()))
		{

			MSG msg;

			bool loadedMainMenu = false;

			bool showLoadingScreen = false;
			bool tempIsInMenu = true;

			Interface MainMenu;


			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);


			if (loadedMainMenu == false)
			{
				MainMenu.Init("MainMenu", ScreenWidth, ScreenHeight, InterfaceAction::Next_Screen::Screen_MainMenu, Graphics_Device);
				loadedMainMenu = true;
			}

			frameBeginC = clock();
			// Enter the message loop
			ZeroMemory(&msg, sizeof(msg));
			while (GameState != State_Quit)
			{






				if ((GetKeyState(VK_RETURN) & 0x80) && tempIsInMenu)
				{
					GameState = State_Game;
					showLoadingScreen = true;
					tempIsInMenu = false;
				}
				if (MainMenu.interfaceController->CheckController().currentScreen == "LoadingScreen" && tempIsInMenu)
				{
					GameState = State_Game;
					showLoadingScreen = true;
					tempIsInMenu = false;
				}



				if (msg.message == WM_QUIT)
				{
					GameState = State_Quit;
				}
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					// Main Loop //


					if (frameTimer >= 1000.0 / 60.0 || GameState == State_Game)
					{
						frameBeginC = clock();


						// Clear the backbuffer and the zbuffer
						Graphics_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

						// Begin the scene
						if (SUCCEEDED(Graphics_Device->BeginScene()))
						{



							if (GameState == State_MainMenu)
							{
								MainMenu.Update(hWnd, ptCurrentMousePosit.x, ptCurrentMousePosit.y, bMouseLeft, bMouseRight);
								MainMenu.Draw();
							}
							if (GameState == State_Game)
							{
								if (showLoadingScreen == true)
								{
									MainMenu.DeleteInterface();
									//display loading screen here

									InitCursor();
									LoadMap(MapFolderString, Graphics_Device);
									showLoadingScreen = false;
									MainMenu.InitEditorInterface();
								}
								Render();





								MainMenu.Update(hWnd, ptCurrentMousePosit.x, ptCurrentMousePosit.y, bMouseLeft, bMouseRight);
								MainMenu.Draw();

							}


							// End the scene
							Graphics_Device->EndScene();
						}

						// Present the backbuffer contents to the display
						Graphics_Device->Present(NULL, NULL, NULL, NULL);
					}

					frameEndC = clock();
					frameTimer = difftime(frameEndC, frameBeginC);


				}
			}


		}
	}

	UnregisterClass(TEXT(" "), wc.hInstance);
	return 0;
}
