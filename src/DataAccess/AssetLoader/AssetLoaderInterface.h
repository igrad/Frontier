#pragma once

#include <QObject>

namespace Assets
{
   class AssetLoaderInterface: public QObject
   {
      Q_OBJECT

   public:
      ~AssetLoaderInterface() = default;

   public slots:
      virtual void LoadImageAsset(const QString& path) = 0;
      virtual void LoadFontAsset(const QString& path) = 0;

   signals:
      void ImageAssetLoaded(const QString& id, const QImage& image);
      void FontAssetLoaded(const QString& id, const QFont& font);
   };
}
