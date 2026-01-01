#pragma once

#include "AssetId.h"

#include <QObject>

class AssetLoaderInterface: public QObject
{
   Q_OBJECT

public:
   ~AssetLoaderInterface() = default;

public slots:
   virtual void LoadImageAsset(const AssetId& id, const QString& path) = 0;
   virtual void LoadFontAsset(const AssetId& id, const QString& path) = 0;

signals:
   void ImageAssetLoaded(const AssetId& id, const QImage& image);
   void FontAssetLoaded(const AssetId& id, const QFont& font);
};
