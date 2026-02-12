#pragma once

#include "AssetClientInterface.h"
#include <AssetId.h>

#include <QPixmap>
#include <QObject>

namespace Assets
{
   class AssetManager;

   class AssetClient: public AssetClientInterface
   {
      Q_OBJECT

   public:
      explicit AssetClient(QObject* parent = nullptr);
      ~AssetClient() = default;

      void LoadFont(Assets::FontName name) override;
      void LoadFont(const QString& path) override;
      void LoadImage(Assets::ImageName name) override;
      void LoadImage(const QString& path) override;

      bool IsAssetAvailable(Assets::FontName name) const override;
      bool IsAssetAvailable(Assets::ImageName name) const override;
      bool IsAssetAvailable(const QString& path) const override;
      QFont GetFont(Assets::FontName name) const override;
      QFont GetFont(const QString& path) const override;
      QPixmap GetImage(Assets::ImageName name) const override;
      QPixmap GetImage(const QString& path) const override;

   private slots:
      void HandleFontLoaded(const QString& path, const QFont& font);
      void HandleImageLoaded(const QString& path, const QPixmap& pixmap);

   private:
      void ConnectToAssetManager();

      AssetManager* Manager;
      QSet<QString> LoadingImages;
      QHash<QString, QPixmap> BatchLoadInProgress;
      QSet<QString> LoadingFonts;
   };
}
