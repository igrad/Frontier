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

   // Build DataAccessThreadManager
   // Build DA thread
   // Move DATM to DA thread
   // Wait for Enterprise to start
   // Enterprise must select RAM or disk memory
   // Enterprise passes DB location selection to DATM
   // DATM caches the bool
   // Enterprise starts the DA thread
   // DATM sets the bool passed down to SettingsService as it's created
   // Enterprise waits for resume press before starting Backend thread

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
      void UseRAMDatabases(bool useRAM);

   public slots:
      void HandleSuspend();
      void HandleResume();
      void HandleDatabaseStarted();
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
