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

	this->parent = currentParent;

	this->controller = currentController;

	this->interfaceName = currentInterfaceName;
	this->name = currentName;


	this->screenWidth = Sw;
	this->screenHeight = Sh;



	this->percentOffset.x = percentOffsetW;
	this->percentOffset.y = percentOffsetH;

	this->percentDimensions.x = percentSizeW;
	this->percentDimensions.y = percentSizeH;


	this->pixelDimensions = this->GetParentPixelDimensions();
	this->pixelOffset = this->GetParentPixelOffset();

	this->upperLeft.x = pixelOffset.x;
	this->upperLeft.y = pixelOffset.y;

	this->lowerRight.x = pixelOffset.x + pixelDimensions.x;
	this->lowerRight.y = pixelOffset.y + pixelDimensions.y;

	this->baseWidth = 1;
	this->baseHeight = 1;



	string textureFilenameString = filename;

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

void InterfaceObject::SetPosition(D3DXVECTOR3 position)
{



	backgroundPosition.x = ((screenWidth / baseWidth)*position.x)*screenWidth;
	backgroundPosition.y = ((screenHeight / baseHeight)*position.y)*screenHeight;
	backgroundPosition.z = position.z;//z does not do anything in terms of screen coordinates, so keep its value at 1

	this->pixelOffset.x = (screenWidth)*position.x;
	this->pixelOffset.y = (screenHeight)*position.y;


}

void InterfaceObject::SetScale(D3DXVECTOR3 scale)
{

	backgroundScale.x = (screenWidth / baseWidth)*scale.x;
	backgroundScale.y = (screenHeight / baseHeight)*scale.y;
	backgroundScale.z = scale.z;

	this->pixelDimensions.x = (screenWidth)*scale.x;
	this->pixelDimensions.y = (screenHeight)*scale.y;

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
	if (this->parent != NULL)
	{

		tempScreenSize = this->parent->GetRootScreenSize();
		return tempScreenSize;
	}
	else
	{
		tempScreenSize.x = this->screenWidth;
		tempScreenSize.y = this->screenHeight;
		return tempScreenSize;
	}
}

D3DXVECTOR2 InterfaceObject::GetParentPixelOffset()
{

	if (this->parent != NULL)
	{
		D3DXVECTOR2 tempOffset;
		tempOffset = this->parent->GetParentPixelOffset();

		D3DXVECTOR2 tempDimensions;
		tempDimensions = this->parent->GetParentPixelDimensions();


		tempOffset.x += (percentOffset.x * tempDimensions.x);
		tempOffset.y += (percentOffset.y * tempDimensions.y);
		return tempOffset;
	}
	else
	{
		D3DXVECTOR2 tempOffset;
		tempOffset.x = this->percentOffset.x * this->screenWidth;
		tempOffset.y = this->percentOffset.y * this->screenHeight;
		return tempOffset;
	}


}

D3DXVECTOR2 InterfaceObject::GetParentPixelDimensions()
{
	//screen size times all parent dimensions

	//first get the combined dimensions
	D3DXVECTOR2 tempPercentDimensions;
	tempPercentDimensions = this->GetParentPercentDimensions();

	//now get the root resolution
	D3DXVECTOR2 tempScreenSize;
	tempScreenSize = this->GetRootScreenSize();

	D3DXVECTOR2 tempPixelDimensions;
	tempPixelDimensions.x = tempPercentDimensions.x * tempScreenSize.x;
	tempPixelDimensions.y = tempPercentDimensions.y * tempScreenSize.y;

	return tempPixelDimensions;


}


D3DXVECTOR2 InterfaceObject::GetParentPercentDimensions()
{
	if (this->parent != NULL)
	{
		D3DXVECTOR2 tempDimensions;
		tempDimensions = this->parent->GetParentPercentDimensions();
		tempDimensions.x *= percentDimensions.x;
		tempDimensions.y *= percentDimensions.y;
		return tempDimensions;
	}
	else
	{
		return this->percentDimensions;
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
	//if(mX > this->upperLeft.x && mX < this->lowerRight.x && mY > this->upperLeft.y && mY < this->lowerRight.y)
	//{
	this->Interact(mX, mY, mL, mR);
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

void InterfaceObject::Interact(double mX, double mY, bool mL, bool mR)
{
	int sjghskdj = 0;
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
			tempDim.x = pixelDimensions.x / interfaceSprites[i]->textureWidth;
			tempDim.y = pixelDimensions.y / interfaceSprites[i]->textureHeight;
			tempDim.z = 1;
			tempOff.x = pixelOffset.x;
			tempOff.y = pixelOffset.y;
			tempOff.z = 1;

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
	this->backgroundTexture->Release();
	this->backgroundSprite->Release();
	for (int i2 = 0; i2 < this->interfaceSprites.size(); i2++)
	{
		this->interfaceSprites[i2]->sprite->Release();
		this->interfaceSprites[i2]->texture->Release();
	}

	if (this->interfaceObjects.size() > 0)
	{
		int tempSize = this->interfaceObjects.size();
		for (int i = 0; i < tempSize; i++)
		{
			this->interfaceObjects[i]->DeleteObject();
		}
		for (int i = 0; i < tempSize; i++)
		{
			delete this->interfaceObjects.back();
			this->interfaceObjects.pop_back();
		}
		this->interfaceObjects.clear();
	}
}
