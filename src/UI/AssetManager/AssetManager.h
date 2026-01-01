#pragma once

#include <AssetLoaderInterface.h>

#include <Utilities/XPtr.h>

#include <QHash>
#include <QSet>

class AssetManager: public QObject
{
   Q_OBJECT

public:
   static AssetManager* GetInstance();

   AssetManager(XPtr<AssetLoaderInterface> loader,
                QObject* parent = nullptr);
   ~AssetManager();

   AssetId RequestFont(const QString& path, QObject* requester);
   AssetId RequestImage(const QString& path, QObject* requester);

signals:
   void FontLoaded(const AssetId& id, const QFont& font);
   void ImageLoaded(const AssetId& id, const QPixmap& pixmap);

private slots:
   void HandleFontAssetLoaded(const AssetId& id, const QFont& font);
   void HandleImageAssetLoaded(const AssetId& id, const QImage& image);

private:
   static AssetManager* Instance;

   bool IsInImageCache(const AssetId& id) const;
   bool IsInFlight(const AssetId& id) const;

   XPtr<AssetLoaderInterface> Loader;
   QHash<AssetId, QPixmap> PixmapCache;
   QSet<AssetId> InFlight;
};
