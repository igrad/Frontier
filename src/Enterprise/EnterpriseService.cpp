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
   , FrontierStarted(false)
   , FrontierSuspended(false)
{
   connect(Window, &EnterpriseWindow::Resume,
           this, &EnterpriseService::HandleResume);
   connect(Window, &EnterpriseWindow::Suspend,
           this, &EnterpriseService::HandleSuspend);
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
   if(FrontierStarted && !FrontierSuspended)
   {
      FrontierSuspended = true;
      SuspendTimer = QDeadlineTimer();
      BackendThread->wait(SuspendTimer);
   }
}

void EnterpriseService::HandleResume()
{
   if(!FrontierStarted)
   {
      FrontierStarted = true;
      FrontierSuspended = false;
      BackendThread->start();
   }
   else if(FrontierSuspended)
   {
      FrontierSuspended = false;
      SuspendTimer.setRemainingTime(0);
   }
}
