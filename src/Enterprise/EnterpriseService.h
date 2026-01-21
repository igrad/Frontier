#pragma once

#include <QDeadlineTimer>
#include <QObject>

class DataAccessThreadManager;
struct DisplayConfigEvent;

namespace Settings
{
   class SettingsClientInterface;
}

namespace Enterprise
{
   class EnterpriseWindow;
   class EnterpriseWin32APIController;

   class EnterpriseService: public QObject
   {
      Q_OBJECT

   public:
      EnterpriseService(DataAccessThreadManager* dataAccess,
                        QObject* parent = nullptr);
      ~EnterpriseService();

      void SetDataAccessThread(QThread* dataAccessThread);
      void SetBackendThread(QThread* backendThread);

   signals:
      void FrontierStarted();
      void DisplayInfoModified(const DisplayConfigEvent& event);

   public slots:
      void HandleSuspend();
      void HandleResume();
      void HandleDatabaseStarted();
      void HandleDataAccessThreadStarted();
      void HandleShellWindowClosed();

   private:
      DataAccessThreadManager* DataAccess;
      QThread* DataAccessThread;
      QThread* BackendThread;

      Settings::SettingsClientInterface* SettingsClient;
      EnterpriseWin32APIController* APIController;
      EnterpriseWindow* Window;

      QDeadlineTimer SuspendTimer;

      bool Started;
      bool Suspended;
   };
}
