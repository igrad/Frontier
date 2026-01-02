#pragma once

#include "AssetId.h"

#include <QObject>

class AssetLoaderInterface: public QObject
{
   Q_OBJECT

public:
   ~AssetLoaderInterface() = default;

public slots:
   virtual void LoadImageAsset(const Assets::AssetId& id, const QString& path) = 0;
   virtual void LoadFontAsset(const Assets::AssetId& id, const QString& path) = 0;

signals:
   void ImageAssetLoaded(const Assets::AssetId& id, const QImage& image);
   void FontAssetLoaded(const Assets::AssetId& id, const QFont& font);
};
