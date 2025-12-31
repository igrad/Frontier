#include "DataAccessThreadManager.h"

#include <SettingsService.h>

#include <QThread>

DataAccessThreadManager::DataAccessThreadManager()
   : TheSettingsService(nullptr)
{
}

void DataAccessThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &DataAccessThreadManager::HandleDataAccessThreadStarted,
           Qt::UniqueConnection);
}

Settings::SettingsService* DataAccessThreadManager::GetTheSettingsService()
{
   return TheSettingsService;
}

void DataAccessThreadManager::HandleUIConnectedToComponents()
{
   TheSettingsService->FetchAllSettings();
}


void DataAccessThreadManager::HandleDataAccessThreadStarted()
{
   LogInfo("Handling DataAccessThread started");

   TheSettingsService = new Settings::SettingsService(this);

   emit DataAccessThreadStarted();
}
