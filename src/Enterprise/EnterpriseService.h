#pragma once

#include <QDeadlineTimer>
#include <QObject>

class DataAccessThreadManager;

namespace Settings
{
   class SettingsClientInterface;
}

namespace Enterprise
{
   class EnterpriseWindow;

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
      EnterpriseWindow* Window;

      QDeadlineTimer SuspendTimer;

      bool Started;
      bool Suspended;
   };
}
