#pragma once

#include <QThread>

namespace Assets
{
   class AssetLoaderInterface;
}

namespace Settings
{
   class SettingsService;
}

class DataAccessThreadManager: public QObject
{
   Q_OBJECT

public:
   DataAccessThreadManager();
   ~DataAccessThreadManager() = default;

   void AssignToThread(QThread* thread);

   void Start();

   Assets::AssetLoaderInterface* GetTheAssetLoader() const;
   Settings::SettingsService* GetTheSettingsService() const;

signals:
   void DataAccessThreadStarted();
   void PassAssetLoader(Assets::AssetLoaderInterface* TheAssetLoader);

public slots:
   void HandleUIConnectedToComponents();
   void HandleRequestAssetLoader();

private slots:
   void HandleDataAccessThreadStarted();

private:
   Assets::AssetLoaderInterface* TheAssetLoader;
   Settings::SettingsService* TheSettingsService;
};
