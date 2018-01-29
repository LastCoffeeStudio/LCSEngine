#include "AssetTexture.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilut.h"

AssetTexture::AssetTexture()
{
}

AssetTexture::AssetTexture(ILinfo info) : width(info.Width), height(info.Height), depth(info.Depth), format(info.Format)
{
}

AssetTexture::~AssetTexture()
{
}
