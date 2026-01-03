#include "DataAccessThreadManager.h"

#include <AssetLoader.h>
#include <SettingsService.h>

#include <QThread>

DataAccessThreadManager::DataAccessThreadManager()
   : TheAssetLoader(nullptr)
   , TheSettingsService(nullptr)
{
}

void DataAccessThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &DataAccessThreadManager::HandleDataAccessThreadStarted,
           Qt::UniqueConnection);
}

Assets::AssetLoaderInterface* DataAccessThreadManager::GetTheAssetLoader() const
{
   return TheAssetLoader;
}

Settings::SettingsService* DataAccessThreadManager::GetTheSettingsService() const
{
   return TheSettingsService;
}

void DataAccessThreadManager::HandleUIConnectedToComponents()
{
   TheSettingsService->FetchAllSettings();
}

void DataAccessThreadManager::HandleRequestAssetLoader()
{
   emit PassAssetLoader(TheAssetLoader);
}

void DataAccessThreadManager::HandleDataAccessThreadStarted()
{
   LogInfo("Handling DataAccessThread started");

   TheAssetLoader = new Assets::AssetLoader(this);
   TheSettingsService = new Settings::SettingsService(this);

   emit DataAccessThreadStarted();
}
