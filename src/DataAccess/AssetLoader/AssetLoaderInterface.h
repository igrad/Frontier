#pragma once

#include <QObject>

class AssetLoaderInterface: public QObject
{
   Q_OBJECT

public:
   ~AssetLoaderInterface() = default;

public slots:
   virtual void LoadImageAsset(const QString& path) = 0;
   virtual void LoadFontAsset(const QString& path) = 0;

signals:
   void ImageAssetLoaded(const QString& path, const QImage& image);
   void FontAssetLoaded(const QString& path, const QFont& font);
};
