#pragma once

#include "Assets.h"
#include <AssetId.h>

#include <QPixmap>
#include <QObject>

class AssetManager;

class AssetClient: public QObject
{
   Q_OBJECT

public:
   AssetClient(const QString& owner, QObject* parent = nullptr);
   ~AssetClient() = default;

   void BatchLoadImages(const QList<Assets::ImageName>& names);
   void LoadFont(Assets::FontName name);
   void LoadImage(Assets::ImageName name);

   bool IsAssetAvailable(Assets::FontName name) const;
   bool IsAssetAvailable(Assets::ImageName name) const;
   QFont GetFont(Assets::FontName name) const;
   QPixmap GetImage(Assets::ImageName name) const;

signals:
   void BatchLoadImagesReady(const QHash<Assets::ImageName, QPixmap>& assets);
   void ImageReady(const Assets::ImageName name, const QPixmap& pixmap);
   void FontReady(const Assets::FontName name, const QFont& font);

private slots:
   void HandleFontLoaded(const Assets::AssetId& id, const QFont& font);
   void HandleImageLoaded(const Assets::AssetId& id, const QPixmap& pixmap);

private:
   void PrivateLoadImage(Assets::ImageName name, bool batch);
   void CheckBatchReady();
   void ConnectToAssetManager();

   AssetManager* Manager;
   QString Owner;
   QSet<Assets::ImageName> BatchLoadNames;
   QHash<Assets::AssetId, Assets::ImageName> LoadingImages;
   QHash<Assets::ImageName, QPixmap> BatchLoadInProgress;
   QHash<Assets::AssetId, Assets::FontName> LoadingFonts;
};
