#include "ArgParser.h"

#include <Logging/Logger.h>
#include <Log.h>
#include <BackendThreadManager/BackendThreadManager.h>
#include <DataAccess/DataAccessThreadManager.h>
#include <UIManager.h>
#include <Enterprise/EnterpriseService.h>
#include <WindowsEventMessageFilter.h>

#include <QApplication>
#include <QThread>

namespace
{
   std::unique_ptr<Logger> LOGGER = nullptr;
   DataAccessThreadManager* DATA_ACCESS_THREAD_MANAGER = nullptr;
   BackendThreadManager* BACKEND_THREAD_MANAGER = nullptr;
   UIManager* UI_MANAGER = nullptr;
   Enterprise::EnterpriseService* ENTERPRISE = nullptr;
}

void TearDownComponents()
{
   if(ENTERPRISE)
   {
      ENTERPRISE->deleteLater();
   }

   if(BACKEND_THREAD_MANAGER)
   {
      BACKEND_THREAD_MANAGER->deleteLater();
   }

   if(DATA_ACCESS_THREAD_MANAGER)
   {
      DATA_ACCESS_THREAD_MANAGER->deleteLater();
   }

   if(LOGGER)
   {
      LOGGER->deleteLater();
   }
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

   // Set up data access thread and its components
   DATA_ACCESS_THREAD_MANAGER = new DataAccessThreadManager();
   std::unique_ptr<QThread> dataAccessThread(new QThread());
   DATA_ACCESS_THREAD_MANAGER->AssignToThread(dataAccessThread.get());

   // Set up backend thread and its components
   BACKEND_THREAD_MANAGER = new BackendThreadManager(DATA_ACCESS_THREAD_MANAGER);
   std::unique_ptr<QThread> backendThread(new QThread());
   BACKEND_THREAD_MANAGER->AssignToThread(backendThread.get());

   // Assign control to Enterprise, if it's enabled
   const bool enterprise = ArgParser::RunningWithEnterprise();
   if(enterprise)
   {
      ENTERPRISE = new Enterprise::EnterpriseService(DATA_ACCESS_THREAD_MANAGER);
      ENTERPRISE->SetDataAccessThread(dataAccessThread.get());
      ENTERPRISE->SetBackendThread(backendThread.get());
   }
   else
   {
      dataAccessThread->start();
      backendThread->start();
   }

   // Set up UI components
   UI_MANAGER = new UIManager(DATA_ACCESS_THREAD_MANAGER,
                              BACKEND_THREAD_MANAGER,
                              static_cast<WindowsAPIInterface*>(&windowsAPI));

   if(enterprise)
   {
      QObject::connect(UI_MANAGER, &UIManager::ShellWindowClosed,
                       ENTERPRISE, &Enterprise::EnterpriseService::HandleShellWindowClosed);
   }

   // Execute
   const int rVal = app.exec();

   // Tear down
   TearDownComponents();
   backendThread->quit();
   backendThread->wait();
   dataAccessThread->quit();
   dataAccessThread->wait();
   return rVal;
}
