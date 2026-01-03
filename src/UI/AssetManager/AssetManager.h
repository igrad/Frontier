#pragma once

#include <AssetLoaderInterface.h>

#include <Utilities/XPtr.h>

#include <QHash>
#include <QSet>

namespace Assets
{
   class AssetClient;

   class AssetManager final: public QObject
   {
      Q_OBJECT

   public:
      static AssetManager* GetInstance();

      AssetManager(XPtr<AssetLoaderInterface> loader,
         QObject* parent = nullptr);
      ~AssetManager();

      bool IsAssetAvailable(const QString& id) const;

      QFont GetFont(const QString& id) const;
      QPixmap GetImage(const QString& id) const;

      void RequestFont(const QString& path, AssetClient* requester);
      void RequestImage(const QString& path, AssetClient* requester);

   signals:
      void FontLoaded(const QString& id, const QFont& font);
      void ImageLoaded(const QString& id, const QPixmap& pixmap);

   private slots:
      void HandleFontAssetLoaded(const QString& id, const QFont& font);
      void HandleImageAssetLoaded(const QString& id, const QImage& image);

   private:
      static AssetManager* Instance;

      bool IsInFontCache(const QString& id) const;
      bool IsInImageCache(const QString& id) const;
      bool IsInFlight(const QString& id) const;
      bool IsFontLoaded(const QString& name) const;

      XPtr<AssetLoaderInterface> Loader;
      QHash<QString, QPair<QString, QPixmap>> PixmapCache;
      QHash<QString, QString> FontCache; 	// Id, font family
      QHash<QString, QString> InFlight; 	// Id, path
   };
}
