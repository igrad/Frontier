#pragma once

#include <QObject>

namespace Assets{
   class AssetClientInterface;
}

namespace TaskBar
{
   class TaskBarAssetProxy: public QObject
   {
      Q_OBJECT

   public:
      explicit TaskBarAssetProxy(QObject* parent = nullptr);
      ~TaskBarAssetProxy() = default;

      void SetAssetClient(Assets::AssetClientInterface* assetClient);

      void LoadStartButtonImage(const QString& path);

   signals:
      void StartButtonImageReady(const QPixmap& image);

   private slots:
      void HandleImageReady(const QString& name, const QPixmap& pixmap);

   private:
      Assets::AssetClientInterface* Client;
      QString StartButtonPath;
   };
}
