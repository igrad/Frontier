#pragma once

#include <QDeadlineTimer>
#include <QObject>

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
      EnterpriseService(QObject* parent = nullptr);
      ~EnterpriseService();

      void SetBackendThread(QThread* backendThread);

   signals:
      void FrontierStarted();

   public slots:
      void HandleSuspend();
      void HandleResume();

   private:
      QThread* BackendThread;
      Settings::SettingsClientInterface* SettingsClient;
      EnterpriseWindow* Window;

      QDeadlineTimer SuspendTimer;

      bool Started;
      bool Suspended;
   };
}
