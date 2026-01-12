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
      void FailedToLoadAsset(const QString& path);
      void FileNotFound(const QString& path);
      void FontAssetLoaded(const QString& path, const QFont& font);
      void ImageAssetLoaded(const QString& path, const QImage& image);
   };
}
