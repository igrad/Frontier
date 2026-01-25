#pragma once

#include "Assets.h"

#include <QObject>

namespace Assets
{
   class AssetManager;

   class AssetClientInterface: public QObject
   {
      Q_OBJECT

   public:
      ~AssetClientInterface() = default;

      virtual void LoadFont(Assets::FontName name) = 0;
      virtual void LoadFont(const QString& path) = 0;
      virtual void LoadImage(Assets::ImageName name) = 0;
      virtual void LoadImage(const QString& path) = 0;

      virtual bool IsAssetAvailable(Assets::FontName name) const = 0;
      virtual bool IsAssetAvailable(Assets::ImageName name) const = 0;
      virtual bool IsAssetAvailable(const QString& path) const = 0;
      virtual QFont GetFont(Assets::FontName name) const = 0;
      virtual QFont GetFont(const QString& path) const = 0;
      virtual QPixmap GetImage(Assets::ImageName name) const = 0;
      virtual QPixmap GetImage(const QString& path) const = 0;

   signals:
      void ImageReady(const QString& name, const QPixmap& pixmap);
      void FontReady(const QString& name, const QFont& font);
   };
}
