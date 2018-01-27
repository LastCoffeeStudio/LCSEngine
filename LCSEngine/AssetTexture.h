#pragma once

class ILinfo;

class AssetTexture
{
public:
	AssetTexture();
	AssetTexture(ILinfo info);
	~AssetTexture();

public:
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;
};

