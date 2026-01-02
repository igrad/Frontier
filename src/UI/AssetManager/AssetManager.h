#pragma once

#include <AssetLoaderInterface.h>

#include <Utilities/XPtr.h>

#include <QHash>
#include <QSet>

class AssetClient;

class AssetManager final: public QObject
{
   Q_OBJECT

public:
   static AssetManager* GetInstance();

   AssetManager(XPtr<AssetLoaderInterface> loader,
                QObject* parent = nullptr);
   ~AssetManager();

   bool IsAssetAvailable(const Assets::AssetId& id) const;

   QFont GetFont(const Assets::AssetId& id) const;
   QPixmap GetImage(const Assets::AssetId& id) const;

   Assets::AssetId RequestFont(const QString& path, AssetClient* requester);
   Assets::AssetId RequestImage(const QString& path, AssetClient* requester);

signals:
   void FontLoaded(const Assets::AssetId& id, const QFont& font);
   void ImageLoaded(const Assets::AssetId& id, const QPixmap& pixmap);

private slots:
   void HandleFontAssetLoaded(const Assets::AssetId& id, const QFont& font);
   void HandleImageAssetLoaded(const Assets::AssetId& id, const QImage& image);

private:
   static AssetManager* Instance;

   bool IsInFontCache(const Assets::AssetId& id) const;
   bool IsInImageCache(const Assets::AssetId& id) const;
   bool IsInFlight(const Assets::AssetId& id) const;
   bool IsFontLoaded(const QString& name) const;

   XPtr<AssetLoaderInterface> Loader;
   QHash<Assets::AssetId, QPair<QString, QPixmap>> PixmapCache;
   QHash<Assets::AssetId, QString> FontCache; 	// Id, font family
   QHash<Assets::AssetId, QString> InFlight; 	// Id, path
};
