#pragma once

#include "AssetLoaderInterface.h"

class AssetLoader: public AssetLoaderInterface
{
   Q_OBJECT

public:
   explicit AssetLoader(QObject* parent = nullptr);
   ~AssetLoader() = default;

public slots:
   void LoadImageAsset(const AssetId& id, const QString& path) override;
   void LoadFontAsset(const AssetId& id, const QString& path) override;
};
