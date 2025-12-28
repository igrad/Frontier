#include "EnterpriseService.h"
#include "EnterpriseWindow.h"

#include <SettingsService/SettingsClient.h>

#include <QThread>

using namespace Enterprise;

EnterpriseService::EnterpriseService(QObject* parent)
   : QObject(parent)
   , BackendThread(nullptr)
   , SettingsClient(new Settings::SettingsClient("Enterprise"))
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
}

EnterpriseService::~EnterpriseService()
{
   Window->deleteLater();
}

void EnterpriseService::SetBackendThread(QThread* backendThread)
{
   BackendThread = backendThread;
}

void EnterpriseService::HandleSuspend()
{
   if(Started && !Suspended)
   {
      Suspended = true;
      SuspendTimer = QDeadlineTimer();
      BackendThread->wait(SuspendTimer);
   }
}

void EnterpriseService::HandleResume()
{
   if(!Started)
   {
      Started = true;
      Suspended = false;
      emit FrontierStarted();
      BackendThread->start();
   }
   else if(Suspended)
   {
      Suspended = false;
      SuspendTimer.setRemainingTime(0);
   }
}
