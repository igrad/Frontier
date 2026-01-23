#include "EnterpriseService.h"
#include "EnterpriseWindow.h"

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

   // This is a quirk to ensure we can pass the display info to Frontier before
   // the backend thread is started and the DisplaysManager is created.
   qRegisterMetaType<DisplayConfigEvent>("DisplayConfigEvent");
   connect(Window, &EnterpriseWindow::DisplayInfoModified,
           this, &EnterpriseService::DisplayInfoModified);
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
