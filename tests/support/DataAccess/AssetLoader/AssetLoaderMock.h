#pragma once

#include <AssetLoaderInterface.h>

#include <gmock/gmock.h>

namespace Assets
{
   class AssetLoaderMock: public AssetLoaderInterface
{
   Q_OBJECT

public:
   MOCK_METHOD(void, LoadFontAsset, (const QString&), (override));
   MOCK_METHOD(void, LoadImageAsset, (const QString&), (override));

   void EmitFailedToLoadAsset(const QString& path);
   void EmitFileNotFound(const QString& path);
   void EmitFontAssetLoaded(const QString& path, const QFont& font);
   void EmitImageAssetLoaded(const QString& path, const QImage& image);
};
}
