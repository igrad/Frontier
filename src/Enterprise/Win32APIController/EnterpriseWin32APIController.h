#pragma once

#include <DisplayInfo.h>

#include <QObject>

namespace Enterprise
{
   class EnterpriseWin32APIController: public QObject
   {
      Q_OBJECT

   public:
      explicit EnterpriseWin32APIController(QObject* parent = nullptr);
      ~EnterpriseWin32APIController() = default;

   signals:
      void DisplayConfigChanged(const DisplayConfigEvent& event);

   public slots:
      void HandleDisplayInfoModified(const DisplayConfigEvent& event);

   private:
      void AddDisplay(const DisplayInfo& info);
   };
}
