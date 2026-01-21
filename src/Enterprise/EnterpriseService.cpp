#include "EnterpriseService.h"
#include "EnterpriseWindow.h"
#include <Win32APIController/EnterpriseWin32APIController.h>

#include <DataAccessThreadManager.h>
#include <SettingsService/SettingsClient.h>

#include <QThread>

using namespace Enterprise;

EnterpriseService::EnterpriseService(DataAccessThreadManager* dataAccess,
                                     QObject* parent)
   : QObject(parent)
   , DataAccess(dataAccess)
   , DataAccessThread(nullptr)
   , BackendThread(nullptr)
   , SettingsClient(nullptr)
   , APIController(new EnterpriseWin32APIController(this))
   , Window(new EnterpriseWindow())
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
   connect(Window, &EnterpriseWindow::DisplayInfoModified,
           APIController, &EnterpriseWin32APIController::HandleDisplayInfoModified);
}

EnterpriseService::~EnterpriseService()
{
   Window->deleteLater();
}

void EnterpriseService::SetDataAccessThread(QThread* dataAccessThread)
{
   LogInfo("DataAccess thread set");
   DataAccessThread = dataAccessThread;
   connect(DataAccessThread, &QThread::started,
           this, &EnterpriseService::HandleDataAccessThreadStarted);
   connect(DataAccess, &DataAccessThreadManager::DataAccessThreadStarted,
           Window, &EnterpriseWindow::DataAccessThreadStarted);
}

void EnterpriseService::SetBackendThread(QThread* backendThread)
{
   LogInfo("Backend thread set");
   BackendThread = backendThread;
}

void EnterpriseService::HandleSuspend()
{
   if(Started && !Suspended)
   {
      LogInfo("Suspending Frontier");
      Suspended = true;
      SuspendTimer = QDeadlineTimer();
      BackendThread->wait(SuspendTimer);
   }
}

void EnterpriseService::HandleResume()
{
   LogInfo("resume");
   if(!Started)
   {
      LogInfo("Starting Frontier");
      Started = true;
      Suspended = false;
      BackendThread->start();
      emit FrontierStarted();
   }
   else if(Suspended)
   {
      LogInfo("Resuming Frontier");
      Suspended = false;
      SuspendTimer.setRemainingTime(0);
   }
}

void EnterpriseService::HandleDatabaseStarted()
{
   LogInfo("Database started");
   DataAccessThread->start();
}

void EnterpriseService::HandleDataAccessThreadStarted()
{
   SettingsClient = new Settings::SettingsClient("Enterprise", this);
   Window->SetSettingsClient(SettingsClient);
}

void EnterpriseService::HandleShellWindowClosed()
{
   if(Window)
   {
      Window->close();
   }
}
