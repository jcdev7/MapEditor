#include "InterfaceObject.h"


InterfaceObject::InterfaceObject(void)
{
}

InterfaceObject::~InterfaceObject(void)
{
}

void InterfaceObject::Init(string currentInterfaceName, string currentName, LPDIRECT3DDEVICE9 GraphicsDevice, int Sw, int Sh, double percentOffsetW, double percentOffsetH, double percentSizeW, double percentSizeH, string filename, InterfaceObject *currentParent, InterfaceController *currentController)
{

	Graphics_Device = GraphicsDevice;

	parent = currentParent;

	controller = currentController;

	interfaceName = currentInterfaceName;
	name = currentName;



	percentOffset.x = percentOffsetW;
	percentOffset.y = percentOffsetH;

	percentDimensions.x = percentSizeW;
	percentDimensions.y = percentSizeH;

	textureFilename = filename;



	Resize(Sw, Sh);





}


void InterfaceObject::SetInitialScreenDimensions(int Sw, int Sh)
{
	//set initial values here (these can't be changed when resizing)
	initialScreenWidth = Sw;
	initialScreenHeight = Sh;

	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		interfaceObjects[i]->SetInitialScreenDimensions(Sw, Sh);
	}


}

void InterfaceObject::SetPosition(D3DXVECTOR3 position)
{



	backgroundPosition.x = ((screenWidth / baseWidth)*position.x)*screenWidth;
	backgroundPosition.y = ((screenHeight / baseHeight)*position.y)*screenHeight;
	backgroundPosition.z = position.z;//z does not do anything in terms of screen coordinates, so keep its value at 1

	pixelOffset.x = (screenWidth)*position.x;
	pixelOffset.y = (screenHeight)*position.y;


}

void InterfaceObject::SetScale(D3DXVECTOR3 scale)
{

	backgroundScale.x = (screenWidth / baseWidth)*scale.x;
	backgroundScale.y = (screenHeight / baseHeight)*scale.y;
	backgroundScale.z = scale.z;

	pixelDimensions.x = (screenWidth)*scale.x;
	pixelDimensions.y = (screenHeight)*scale.y;

}

void InterfaceObject::SetScreenHeight(int height)
{
	screenHeight = height;
}

void InterfaceObject::SetScreenWidth(int width)
{
	screenWidth = width;
}


D3DXVECTOR2 InterfaceObject::GetRootScreenSize()
{

	D3DXVECTOR2 tempScreenSize;
	if (parent != NULL)
	{

		tempScreenSize = parent->GetRootScreenSize();
		return tempScreenSize;
	}
	else
	{
		tempScreenSize.x = screenWidth;
		tempScreenSize.y = screenHeight;
		return tempScreenSize;
	}
}

D3DXVECTOR2 InterfaceObject::GetParentPixelOffset()
{

	if (parent != NULL)
	{
		D3DXVECTOR2 tempOffset;
		tempOffset = parent->GetParentPixelOffset();

		D3DXVECTOR2 tempDimensions;
		tempDimensions = parent->GetParentPixelDimensions();


		tempOffset.x += (percentOffset.x * tempDimensions.x);
		tempOffset.y += (percentOffset.y * tempDimensions.y);
		return tempOffset;
	}
	else
	{
		D3DXVECTOR2 tempOffset;
		tempOffset.x = percentOffset.x * screenWidth;
		tempOffset.y = percentOffset.y * screenHeight;
		return tempOffset;
	}


}

D3DXVECTOR2 InterfaceObject::GetParentPixelDimensions()
{
	//screen size times all parent dimensions

	//first get the combined dimensions
	D3DXVECTOR2 tempPercentDimensions;
	tempPercentDimensions = GetParentPercentDimensions();

	//now get the root resolution
	D3DXVECTOR2 tempScreenSize;
	tempScreenSize = GetRootScreenSize();

	D3DXVECTOR2 tempPixelDimensions;
	tempPixelDimensions.x = tempPercentDimensions.x * tempScreenSize.x;
	tempPixelDimensions.y = tempPercentDimensions.y * tempScreenSize.y;


	if (lockHeightToWidthRatio == true)
	{
		tempPixelDimensions.y = tempPixelDimensions.x * heightToWidthRatio;
	}
	else if (lockWidthToHeightRatio == true)
	{
		tempPixelDimensions.x = tempPixelDimensions.y * widthToHeightRatio;
	}

	return tempPixelDimensions;


}


D3DXVECTOR2 InterfaceObject::GetParentPercentDimensions()
{
	if (parent != NULL)
	{
		D3DXVECTOR2 tempDimensions;
		tempDimensions = parent->GetParentPercentDimensions();
		tempDimensions.x *= percentDimensions.x;
		tempDimensions.y *= percentDimensions.y;
		return tempDimensions;
	}
	else
	{
		return percentDimensions;
	}
}

void InterfaceObject::SetOffset(D3DXVECTOR2 parentOffset)
{
	pixelOffset.x = parentOffset.x;
	pixelOffset.y = parentOffset.y;
}
void InterfaceObject::SetDimensions(D3DXVECTOR2 parentDimensions)
{
	pixelDimensions.x = parentDimensions.x;
	pixelDimensions.y = parentDimensions.y;
}


void InterfaceObject::Update(double mX, double mY, bool mL, bool mR)
{


	//check if mouse is within the object's dimensions
	//if(mX > upperLeft.x && mX < lowerRight.x && mY > upperLeft.y && mY < lowerRight.y)
	//{
	Interact(mX, mY, mL, mR);
	//}
	//else
	//{
	//do nothing
	//}

	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		interfaceObjects[i]->Update(mX, mY, mL, mR);
	}


}

//resize interface object based on the screen dimensions
void InterfaceObject::Resize(int Sw, int Sh)
{



	screenWidth = Sw;
	screenHeight = Sh;

	pixelDimensions = GetParentPixelDimensions();
	pixelOffset = GetParentPixelOffset();

	upperLeft.x = pixelOffset.x;
	upperLeft.y = pixelOffset.y;

	lowerRight.x = pixelOffset.x + pixelDimensions.x;
	lowerRight.y = pixelOffset.y + pixelDimensions.y;

	baseWidth = 1;
	baseHeight = 1;

	LoadTexture();

	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		interfaceObjects[i]->Resize(Sw, Sh);
	}

}

void InterfaceObject::LoadTexture()
{




	string textureFilenameString = textureFilename;

	char textureFileName[255];
	strcpy_s(textureFileName, textureFilenameString.c_str());

	//create a texture from image file
	if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &backgroundTexture)))
	{
		// If texture is not in current folder, try parent folder
		textureFilenameString = "..\\InterfaceTextures\\" + textureFilenameString;
		strcpy_s(textureFileName, textureFilenameString.c_str());
		if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &backgroundTexture)))
		{
			int kjfghkdf = 0;
			//return E_FAIL;
		}
	}


	D3DSURFACE_DESC desc1;

	backgroundTexture->GetLevelDesc(0, &desc1);

	baseWidth = desc1.Width;
	baseHeight = desc1.Height;



	D3DXCreateSprite(Graphics_Device, &backgroundSprite);



}


void InterfaceObject::Interact(double mX, double mY, bool mL, bool mR)
{

}

void InterfaceObject::Draw()
{

	Graphics_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	

	D3DXVECTOR3 tempDim;
	D3DXVECTOR3 tempOff;

	//need to set scale so the image file's baseWidth/baseHeight fits within the required dimensions
	tempDim.x = pixelDimensions.x / baseWidth;
	tempDim.y = pixelDimensions.y / baseHeight;
	tempDim.z = 1;
	tempOff.x = pixelOffset.x;
	tempOff.y = pixelOffset.y;
	tempOff.z = 1;


	tempDim.x *= (initialScreenWidth / screenWidth);
	tempDim.y *= (initialScreenHeight / screenHeight);

	tempOff.x *= (initialScreenWidth / screenWidth);
	tempOff.y *= (initialScreenHeight / screenHeight);


	D3DXMatrixTranslation(&mTranslation, tempOff.x, tempOff.y, tempOff.z);
	D3DXMatrixScaling(&mScale, tempDim.x, tempDim.y, tempDim.z);
	D3DXMatrixMultiply(&mScaleAndTranslate, &mScale, &mTranslation);
	backgroundSprite->SetTransform(&mScaleAndTranslate);



	D3DXVECTOR3 tempVector = D3DXVECTOR3(0, 0, 0);


	backgroundSprite->Begin(NULL);
	{
		backgroundSprite->Draw(backgroundTexture, NULL, NULL, &tempVector, 0xFFFFFFFF);
	}
	backgroundSprite->End();

	for (int i = 0; i < interfaceSprites.size(); i++)
	{
		if (interfaceSprites[i]->visible == true)
		{
			tempDim.x = pixelDimensions.x / baseWidth;
			tempDim.y = pixelDimensions.y / baseHeight;
			tempDim.z = 1;
			tempOff.x = pixelOffset.x;
			tempOff.y = pixelOffset.y;
			tempOff.z = 1;


			tempDim.x *= (initialScreenWidth / screenWidth);
			tempDim.y *= (initialScreenHeight / screenHeight);

			tempOff.x *= (initialScreenWidth / screenWidth);
			tempOff.y *= (initialScreenHeight / screenHeight);

			D3DXMatrixTranslation(&mTranslation, tempOff.x, tempOff.y, tempOff.z);
			D3DXMatrixScaling(&mScale, tempDim.x, tempDim.y, tempDim.z);
			D3DXMatrixMultiply(&mScaleAndTranslate, &mScale, &mTranslation);
			interfaceSprites[i]->sprite->SetTransform(&mScaleAndTranslate);

			interfaceSprites[i]->sprite->Begin(NULL);
			{
				interfaceSprites[i]->sprite->Draw(interfaceSprites[i]->texture, NULL, NULL, &tempVector, 0xFFFFFFFF);
			}
			interfaceSprites[i]->sprite->End();
		}
	}



	Graphics_Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (int i = 0; i < interfaceObjects.size(); i++)
	{
		interfaceObjects[i]->Draw();
	}

}

void InterfaceObject::DeleteObject()
{
	//delete everything in this object
	backgroundTexture->Release();
	backgroundSprite->Release();
	for (int i2 = 0; i2 < interfaceSprites.size(); i2++)
	{
		interfaceSprites[i2]->sprite->Release();
		interfaceSprites[i2]->texture->Release();
	}

	if (interfaceObjects.size() > 0)
	{
		int tempSize = interfaceObjects.size();
		for (int i = 0; i < tempSize; i++)
		{
			interfaceObjects[i]->DeleteObject();
		}
		for (int i = 0; i < tempSize; i++)
		{
			delete interfaceObjects.back();
			interfaceObjects.pop_back();
		}
		interfaceObjects.clear();
	}
}
