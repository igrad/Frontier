#pragma once

#include <QVBoxLayout>
#include <QWidget>

struct DisplayConfigEvent;

namespace Enterprise
{
   class EnterpriseMonitorWidget;
   class EnterpriseMonitorWindow;

   class EnterpriseHardwareTab: public QWidget
   {
      Q_OBJECT

   public:
      EnterpriseHardwareTab();
      ~EnterpriseHardwareTab() = default;

      void PublishInitialDisplayInfo();

   signals:
      void DisplayInfoModified(const DisplayConfigEvent& event);

   private:
      void BuildUI();

      QVBoxLayout* Layout;
      QHBoxLayout* MonitorBtnLayout;
      EnterpriseMonitorWidget* Monitor1Btn;
      EnterpriseMonitorWidget* Monitor2Btn;
      EnterpriseMonitorWidget* Monitor3Btn;
      EnterpriseMonitorWidget* Monitor4Btn;
   };
}
