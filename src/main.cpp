#include "ArgParser.h"

#include <Logging/Logger.h>
#include <Log.h>
#include <SettingsService.h>
#include <BackendThreadManager/BackendThreadManager.h>
#include <UIManager.h>
#include <Enterprise/EnterpriseService.h>

#include <QApplication>
#include <QThread>

namespace
{
   std::unique_ptr<Logger> LOGGER = nullptr;
   std::unique_ptr<BackendThreadManager> BACKEND_THREAD_MANAGER = nullptr;
   std::unique_ptr<UIManager> UI_MANAGER = nullptr;
   std::unique_ptr<Enterprise::EnterpriseService> ENTERPRISE = nullptr;
}

void TearDownComponents()
{
   LOGGER->deleteLater();
   LOGGER = nullptr;

   BACKEND_THREAD_MANAGER->deleteLater();
   BACKEND_THREAD_MANAGER = nullptr;
}

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   ArgParser argParser;
   argParser.ParseArgs(app);

   if(ArgParser::GetArgAsBool(ArgParser::Arg::Debug, false))
   {
      #ifndef QT_DEBUG
      #define QT_DEBUG
      #endif
   }

   // LOGGER is a special case since we need logging before launching the app
   LOGGER.reset(new Logger(nullptr));
   if(ArgParser::RunningInCleanMode())
   {
      LogInfo("Running in clean mode");
   }
   LogInfo("Launching Frontier");
   LogInfo(QString("Launching with argc: %1 and args").arg(argc));
   for(int iter = 1; iter < argc; ++iter)
   {
      LogInfo(QString("arg %1: %2").arg(iter).arg(argv[iter]));
   }

   // Set up backend thread and its components
   BACKEND_THREAD_MANAGER.reset(new BackendThreadManager());
   std::unique_ptr<QThread> backendThread(new QThread());
   BACKEND_THREAD_MANAGER->AssignToThread(backendThread.get());

   // Assign control to Enterprise, if it's enabled
   const bool enterprise = ArgParser::RunningWithEnterprise();
   if(enterprise)
   {
      ENTERPRISE.reset(new Enterprise::EnterpriseService());
      ENTERPRISE->SetBackendThread(backendThread.get());
   }
   else
   {
      backendThread->start(QThread::NormalPriority);
   }

   // Set up UI components
   UI_MANAGER.reset(new UIManager(BACKEND_THREAD_MANAGER.get()));

   // Execute
   const int rVal = app.exec();

   // Tear down
   // TODO: Cleanly tear down backendThread
   TearDownComponents();
   backendThread->quit();
   backendThread->wait();
   return rVal;
}
