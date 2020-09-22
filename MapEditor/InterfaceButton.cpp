#include "InterfaceButton.h"

InterfaceButton::InterfaceButton(void)
{
}

InterfaceButton::~InterfaceButton(void)
{
}

void InterfaceButton::InitButton(string filename)
{

	InterfaceSprite *tempSprite = new InterfaceSprite();

	tempSprite->visible = false;

	string textureFilenameString = filename;

	char textureFileName[255];
	strcpy_s(textureFileName, textureFilenameString.c_str());

	//create a texture from image file
	if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &tempSprite->texture)))
	{
		// If texture is not in current folder, try parent folder
		textureFilenameString = "..\\InterfaceTextures\\" + textureFilenameString;
		strcpy_s(textureFileName, textureFilenameString.c_str());
		if (FAILED(D3DXCreateTextureFromFile(Graphics_Device, textureFileName, &tempSprite->texture)))
		{
			int fdkjghkjdf = 0;
			//return E_FAIL;
		}
	}


	D3DSURFACE_DESC desc1;

	tempSprite->texture->GetLevelDesc(0, &desc1);

	tempSprite->textureWidth = desc1.Width;
	tempSprite->textureHeight = desc1.Height;



	D3DXCreateSprite(Graphics_Device, &tempSprite->sprite);


	this->interfaceSprites.push_back(tempSprite);


}

void InterfaceButton::Interact(double mX, double mY, bool mL, bool mR)
{
	//check if mouse is within the object's dimensions
	if (mX > this->upperLeft.x && mX < this->lowerRight.x && mY > this->upperLeft.y && mY < this->lowerRight.y)
	{
		if (this->interfaceSprites.size() > 0)
		{
			this->interfaceSprites.back()->visible = true;
		}


		if (mL == true)
		{
			int sjdkhgsdkj = 0;
			pressed = true;
			this->controller->UpdateScreen(this->interfaceName, this->name, 1);
		}
		else
		{
			pressed = false;
		}
	}
	else
	{
		if (this->interfaceSprites.size() > 0)
		{
			this->interfaceSprites.back()->visible = false;
		}
	}




}