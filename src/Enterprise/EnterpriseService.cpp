#include "EnterpriseService.h"
#include "EnterpriseWindow.h"

#include <DataAccessThreadManager.h>
#include <SettingsService/SettingsClient.h>

#include <QThread>
#include <iostream>

using namespace Enterprise;

EnterpriseService::EnterpriseService(DataAccessThreadManager* dataAccess,
                                     QObject* parent)
   : QObject(parent)
   , DataAccess(dataAccess)
   , DataAccessThread(nullptr)
   , BackendThread(nullptr)
   , SettingsClient(nullptr)
   , Window(new EnterpriseWindow(SettingsClient))
   , SuspendTimer()
   , Started(false)
   , Suspended(false)
{
   connect(Window, &EnterpriseWindow::Resume,
           this, &EnterpriseService::HandleResume);
   connect(Window, &EnterpriseWindow::Suspend,
           this, &EnterpriseService::HandleSuspend);
   connect(this, &EnterpriseService::FrontierStarted,
           Window, &EnterpriseWindow::HandleFrontierStarted);
   connect(Window, &EnterpriseWindow::DatabaseStarted,
           this, &EnterpriseService::HandleDatabaseStarted);
}

EnterpriseService::~EnterpriseService()
{
   Window->deleteLater();
}

void EnterpriseService::SetDataAccessThread(QThread* dataAccessThread)
{
   std::cout << "Enterprise - DataAccess thread set" << std::endl;
   DataAccessThread = dataAccessThread;
   connect(DataAccessThread, &QThread::started,
           this, &EnterpriseService::HandleDataAccessThreadStarted);
   connect(Window, &EnterpriseWindow::UseRAMDatabases,
           DataAccess, &DataAccessThreadManager::HandleUseRAMDatabases);
   connect(DataAccess, &DataAccessThreadManager::DataAccessThreadStarted,
           Window, &EnterpriseWindow::DataAccessThreadStarted);
}

void EnterpriseService::SetBackendThread(QThread* backendThread)
{
   std::cout << "Enterprise - Backend thread set" << std::endl;
   BackendThread = backendThread;
}

void EnterpriseService::HandleSuspend()
{
   if(Started && !Suspended)
   {
      std::cout << "Enterprise - Suspending Frontier" << std::endl;
      Suspended = true;
      SuspendTimer = QDeadlineTimer();
      BackendThread->wait(SuspendTimer);
   }
}

void EnterpriseService::HandleResume()
{
   std::cout << "Enterprise - resume" << std::endl;
   if(!Started)
   {
      std::cout << "Enterprise - Starting Frontier" << std::endl;
      Started = true;
      Suspended = false;
      BackendThread->start();
      emit FrontierStarted();
   }
   else if(Suspended)
   {
      std::cout << "Enterprise - Resuming Frontier" << std::endl;
      Suspended = false;
      SuspendTimer.setRemainingTime(0);
   }
}

void EnterpriseService::HandleDatabaseStarted()
{
   std::cout << "Enterprise - Database started" << std::endl;
   DataAccessThread->start();
}

void EnterpriseService::HandleDataAccessThreadStarted()
{
   SettingsClient = new Settings::SettingsClient("Enterprise", this);
}

void EnterpriseService::HandleShellWindowClosed()
{
   if(Window)
   {
      Window->close();
   }
}
