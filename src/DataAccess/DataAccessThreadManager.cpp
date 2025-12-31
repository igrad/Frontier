#include "DataAccessThreadManager.h"

#include <SettingsService.h>

#include <QThread>

DataAccessThreadManager::DataAccessThreadManager()
   : TheSettingsService(nullptr)
   , UseRAMDatabases(false)
{
}

void DataAccessThreadManager::AssignToThread(QThread* thread)
{
   this->moveToThread(thread);

   connect(thread, &QThread::started,
           this, &DataAccessThreadManager::HandleDataAccessThreadStarted,
           Qt::UniqueConnection);
   // connect(thread, &QThread::finished,
   //         this, &QObject::deleteLater,
   //         Qt::UniqueConnection);
}

Settings::SettingsService* DataAccessThreadManager::GetTheSettingsService()
{
   return TheSettingsService;
}

void DataAccessThreadManager::HandleUIConnectedToComponents()
{
   TheSettingsService->FetchAllSettings();
}

void DataAccessThreadManager::HandleUseRAMDatabases(bool useRAM)
{
   LogInfo(QString("Will %1use RAM database, :memory:").arg(useRAM ? "" : "not"));
   UseRAMDatabases = useRAM;
}

void DataAccessThreadManager::HandleDataAccessThreadStarted()
{
   LogInfo(QString("Starting DataAccessThread. Using RAM databases: %1")
              .arg(UseRAMDatabases ? "true" : "false"));
   TheSettingsService = new Settings::SettingsService(UseRAMDatabases, this);

   emit DataAccessThreadStarted();
}
