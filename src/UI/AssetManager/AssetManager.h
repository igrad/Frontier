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

   void RequestFont(const QString& path, QObject* requester);
   void RequestImage(const QString& path, QObject* requester);

signals:
   void FontLoaded(const QString& path, const QFont& font);
   void ImageLoaded(const QString& path, const QPixmap& pixmap);

private slots:
   void HandleFontAssetLoaded(const QString& path, const QFont& font);
   void HandleImageAssetLoaded(const QString& path, const QImage& image);

private:
   static AssetManager* Instance;

   XPtr<AssetLoaderInterface> Loader;
   QHash<QString, QPixmap> PixmapCache;
   QSet<QString> InFlight;
};
