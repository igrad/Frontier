#pragma once

#include "EnterpriseMonitorWindow.h"

#include <DisplayInfo.h>

#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Enterprise
{
   class EnterpriseMonitorWidget: public QWidget
   {
      Q_OBJECT

   public:
      explicit EnterpriseMonitorWidget(QWidget* parent = nullptr);
      ~EnterpriseMonitorWidget() = default;

      void PublishInitialDisplayInfo();

   signals:
      void DisplayInfoModified(const DisplayConfigEvent& event);

   private slots:
      void HandleConfigPressed(bool pressed);
      void HandleActiveCheckBoxChecked(bool checked);
      void HandlePrimaryCheckBoxChecked(bool checked);
      void HandleWindowClosed();
      void HandleDisplayInfoModified();

   private:
      static QMap<unsigned int, EnterpriseMonitorWidget*> MonitorWidgets;
      static QMap<unsigned int, bool> ActiveMonitors;
      static std::unique_ptr<EnterpriseMonitorWindow> Window;

      void CreateUI();
      void ConfigureInfo();
      void UpdateActiveAndPrimaryData();
      void CheckForDisplayInfoModified();

      int DisplayNum;

      QVBoxLayout* WrapperLayout;
      QVBoxLayout* Layout;
      QFrame* Frame;
      QPushButton* ConfigBtn;
      QHBoxLayout* CheckBoxesLayout;
      QCheckBox* ActiveCheckBox;
      QCheckBox* PrimaryCheckBox;
   };
}
