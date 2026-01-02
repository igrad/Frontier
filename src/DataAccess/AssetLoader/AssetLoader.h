#pragma once

#include "AssetLoaderInterface.h"

class AssetLoader: public AssetLoaderInterface
{
   Q_OBJECT

public:
   explicit AssetLoader(QObject* parent = nullptr);
   ~AssetLoader() = default;

public slots:
   void LoadImageAsset(const Assets::AssetId& id, const QString& path) override;
   void LoadFontAsset(const Assets::AssetId& id, const QString& path) override;
};
