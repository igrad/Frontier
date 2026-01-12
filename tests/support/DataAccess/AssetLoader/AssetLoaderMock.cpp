#include "AssetLoaderMock.h"

using namespace Assets;

void AssetLoaderMock::EmitFailedToLoadAsset(const QString& path)
{
   emit FailedToLoadAsset(path);
}

void AssetLoaderMock::EmitFileNotFound(const QString& path)
{
   emit FileNotFound(path);
}

void AssetLoaderMock::EmitFontAssetLoaded(const QString& path, const QFont& font)
{
   emit FontAssetLoaded(path, font);
}

void AssetLoaderMock::EmitImageAssetLoaded(const QString& path, const QImage& image)
{
   emit ImageAssetLoaded(path, image);
}
