#include "DataAccessThreadManager.h"

#include <SettingsService.h>

#include <QThread>

#include <iostream>

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
   UseRAMDatabases = useRAM;
}

void DataAccessThreadManager::HandleDataAccessThreadStarted()
{
   std::cout << "DataAccessThread started" << std::endl;
   TheSettingsService = new Settings::SettingsService(UseRAMDatabases, this);

   emit DataAccessThreadStarted();
}
