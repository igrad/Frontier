#pragma once

#include <AssetClient.h>

#include <QObject>

namespace TaskBar
{
   class TaskBarAssetProxy: public QObject
   {
      Q_OBJECT

   public:
      explicit TaskBarAssetProxy(QObject* parent = nullptr);
      ~TaskBarAssetProxy() = default;

      void LoadStartButtonImage(const QString& path);

   signals:
      void StartButtonImageReady(const QPixmap& image);

   private slots:
      void HandleImageReady(const QString& name, const QPixmap& pixmap);

   private:
      Assets::AssetClient Client;
      QString StartButtonPath;
   };
}
