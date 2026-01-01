#pragma once

#include <QThread>

class AssetLoaderInterface;

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

   AssetLoaderInterface* GetTheAssetLoader() const;
   Settings::SettingsService* GetTheSettingsService() const;

signals:
   void DataAccessThreadStarted();
   void PassAssetLoader(AssetLoaderInterface* TheAssetLoader);

public slots:
   void HandleUIConnectedToComponents();
   void HandleRequestAssetLoader();

private slots:
   void HandleDataAccessThreadStarted();

private:
   AssetLoaderInterface* TheAssetLoader;
   Settings::SettingsService* TheSettingsService;
};
