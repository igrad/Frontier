#pragma once

#include "AssetLoaderInterface.h"

class QFile;

namespace Assets
{
   class AssetLoader: public AssetLoaderInterface
   {
      Q_OBJECT

   public:
      explicit AssetLoader(QObject* parent = nullptr);
      ~AssetLoader() = default;

   public slots:
      void LoadImageAsset(const QString& path) override;
      void LoadFontAsset(const QString& path) override;

   private:
      bool FileExists(const QString& path, const QFile& file);
      bool FileIsReadable(const QString& path, QFile& file);
   };
}
