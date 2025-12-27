#pragma once

#include <QDeadlineTimer>
#include <QObject>

namespace Enterprise
{
   class EnterpriseWindow;

   class EnterpriseService: public QObject
   {
      Q_OBJECT

   public:
      explicit EnterpriseService(QObject* parent = nullptr);
      ~EnterpriseService();

      void SetBackendThread(QThread* backendThread);

   private slots:
      void HandleSuspend();
      void HandleResume();

   private:
      QThread* BackendThread;
      EnterpriseWindow* Window;

      QDeadlineTimer SuspendTimer;

      bool FrontierStarted;
      bool FrontierSuspended;
   };
}
