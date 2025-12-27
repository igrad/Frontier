#include "EnterpriseService.h"
#include "EnterpriseWindow.h"

#include <QThread>

using namespace Enterprise;

EnterpriseService::EnterpriseService(QObject* parent)
   : QObject(parent)
   , BackendThread(nullptr)
   , Window(new EnterpriseWindow())
   , SuspendTimer()
   , FrontierStarted(false)
   , FrontierSuspended(false)
{
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
