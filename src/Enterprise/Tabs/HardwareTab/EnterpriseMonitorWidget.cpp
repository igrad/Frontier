#include "EnterpriseMonitorWidget.h"

#include <Log.h>

using namespace Enterprise;

// IMPORTANT NOTE: DisplayNum and MonitorWidgets are both 1-indexed in this class and
// in EnterpriseMonitorWindow because Microsoft decided to make monitors 1-indexed.

QMap<unsigned int, EnterpriseMonitorWidget*> EnterpriseMonitorWidget::MonitorWidgets;
QMap<unsigned int, bool> EnterpriseMonitorWidget::ActiveMonitors;
std::unique_ptr<EnterpriseMonitorWindow> EnterpriseMonitorWidget::Window(nullptr);

EnterpriseMonitorWidget::EnterpriseMonitorWidget(QWidget* parent)
   : QWidget(parent)
   , DisplayNum(MonitorWidgets.size() + 1)
{
   MonitorWidgets[DisplayNum] = this;

   if(ActiveMonitors.isEmpty())
   {
      for(int iter = 1; iter < 5; ++iter)
      {
         ActiveMonitors[iter] = (iter == 1);
      }
   }

   ConfigureInfo();
   CreateUI();

   LogInfo(QString("Starting EnterpriseMonitorWidget %1")
              .arg(DisplayNum));
}

void EnterpriseMonitorWidget::PublishInitialDisplayInfo()
{
   UpdateActiveAndPrimaryData();
   CheckForDisplayInfoModified();
}

void EnterpriseMonitorWidget::HandleConfigPressed(bool pressed)
{
   Q_UNUSED(pressed);

   if(nullptr == Window)
   {
      Window.reset(new EnterpriseMonitorWindow(DisplayNum));
      connect(Window.get(), &EnterpriseMonitorWindow::Closed,
              this, &EnterpriseMonitorWidget::HandleWindowClosed);
      connect(Window.get(), &EnterpriseMonitorWindow::DisplayInfoModified,
              this, &EnterpriseMonitorWidget::HandleDisplayInfoModified);
   }
}

void EnterpriseMonitorWidget::HandleActiveCheckBoxChecked(bool checked)
{
   ConfigBtn->setDisabled(!checked);

   if(!checked && PrimaryCheckBox->isChecked())
   {
      MonitorWidgets[1]->PrimaryCheckBox->setChecked(true);
   }

   PrimaryCheckBox->setDisabled(!checked);
   UpdateActiveAndPrimaryData();
   CheckForDisplayInfoModified();
}

void EnterpriseMonitorWidget::HandlePrimaryCheckBoxChecked(bool checked)
{
   if(!checked)
   {
      // do nothing
      return;
   }

   for(int iter = 1; iter < 5; ++iter)
   {
      if(MonitorWidgets.contains(iter))
      {
         const bool iterIsActive = MonitorWidgets[iter]->ActiveCheckBox->isChecked();
         MonitorWidgets[iter]->PrimaryCheckBox->setDisabled((DisplayNum == iter) || !iterIsActive);
         MonitorWidgets[iter]->PrimaryCheckBox->setChecked(DisplayNum == iter);

      }
   }

   UpdateActiveAndPrimaryData();
   CheckForDisplayInfoModified();
}

void EnterpriseMonitorWidget::HandleWindowClosed()
{
   if(nullptr != Window)
   {
      disconnect(Window.get(), &EnterpriseMonitorWindow::Closed,
                 this, &EnterpriseMonitorWidget::HandleWindowClosed);
      disconnect(Window.get(), &EnterpriseMonitorWindow::DisplayInfoModified,
                 this, &EnterpriseMonitorWidget::HandleDisplayInfoModified);
      Window.reset(nullptr);
   }
}

void EnterpriseMonitorWidget::HandleDisplayInfoModified()
{
   UpdateActiveAndPrimaryData();
   CheckForDisplayInfoModified();
}

void EnterpriseMonitorWidget::CreateUI()
{
   WrapperLayout = new QVBoxLayout(this);
   Frame = new QFrame();
   Frame->setObjectName(QString("MonitorWidgetFrame").arg(DisplayNum));
   Frame->setStyleSheet(QString("#%1 {border: 2px solid gray; border-radius: 5px;}")
                           .arg(Frame->objectName()));

   WrapperLayout->addWidget(Frame);

   Layout = new QVBoxLayout(Frame);
   ConfigBtn = new QPushButton();
   ConfigBtn->setText(QString("Configure Display %1")
                         .arg(DisplayNum));
   ConfigBtn->setStyleSheet("padding: 10px;");
   if(DisplayNum > 1)
   {
      ConfigBtn->setDisabled(true);
   }
   connect(ConfigBtn, &QPushButton::clicked,
           this, &EnterpriseMonitorWidget::HandleConfigPressed);
   Layout->addWidget(ConfigBtn);

   CheckBoxesLayout = new QHBoxLayout();
   ActiveCheckBox = new QCheckBox();
   ActiveCheckBox->setText("Active");
   connect(ActiveCheckBox, &QCheckBox::checkStateChanged,
           this, &EnterpriseMonitorWidget::HandleActiveCheckBoxChecked);
   PrimaryCheckBox = new QCheckBox();
   PrimaryCheckBox->setText("Primary");
   PrimaryCheckBox->setDisabled(true);
   connect(PrimaryCheckBox, &QCheckBox::checkStateChanged,
           this, &EnterpriseMonitorWidget::HandlePrimaryCheckBoxChecked);
   if(1 == DisplayNum)
   {
      ActiveCheckBox->setChecked(true);
      ActiveCheckBox->setDisabled(true);
      PrimaryCheckBox->setChecked(true);
      PrimaryCheckBox->setDisabled(true);
   }
   CheckBoxesLayout->addWidget(ActiveCheckBox);
   CheckBoxesLayout->addWidget(PrimaryCheckBox);
   Layout->addLayout(CheckBoxesLayout);
}

void EnterpriseMonitorWidget::ConfigureInfo()
{
   DisplayInfo info;
   info.DisplayName = QString("Display #%1").arg(DisplayNum);
   info.ID = QString("DisplayID%1").arg(DisplayNum);
   info.SessionName = QString("\\\\.\\DISPLAY%1").arg(DisplayNum);
   info.Number = DisplayNum;
   info.IsPrimary = (DisplayNum == 1);
   info.Handle = new HMONITOR__();
   info.Rect = QRect(0, 0, 1920, 1080);
   info.XDPI = 24;
   info.YDPI = 24;

   EnterpriseMonitorWindow::DisplaysInfo[DisplayNum] = info;
}

void EnterpriseMonitorWidget::UpdateActiveAndPrimaryData()
{
   QMap<unsigned int, DisplayInfo> displays = EnterpriseMonitorWindow::DisplaysInfo;

   for(int iter = 1; iter < 5; ++iter)
   {
      if(displays.contains(iter) && MonitorWidgets.contains(iter))
      {
         displays[iter].IsPrimary = MonitorWidgets[iter]->PrimaryCheckBox->isChecked();
         ActiveMonitors[iter] = MonitorWidgets[iter]->ActiveCheckBox->isChecked();
      }
   }
}

void EnterpriseMonitorWidget::CheckForDisplayInfoModified()
{
   DisplayConfigEvent event;

   // only send the active monitors!
   // Also make sure this gets sent Frontier startup time. The service will already be made
   // and connected to this signal.
   QPair<DisplayConfigEventType, DisplayInfo> pair;
   for(int iter = 1; iter < EnterpriseMonitorWindow::DisplaysInfo.size() + 1; ++iter)
   {
      const DisplayInfo& info = EnterpriseMonitorWindow::DisplaysInfo[iter];
      if(ActiveMonitors[iter])
      {
         LogInfo(QString("Added monitor %1").arg(iter));
         pair.first = DisplayConfigEventType::Added;
      }
      else
      {
         pair.first = DisplayConfigEventType::Removed;
      }

      pair.second = info;
      event.Displays[info.ID] = pair;
   }

   const int added = std::count_if(event.Displays.constBegin(),
                                   event.Displays.constEnd(),
                                   [&](const QPair<DisplayConfigEventType, DisplayInfo>& pair){
                                      return pair.first == DisplayConfigEventType::Added;
                                   });
   LogInfo(QString("Display config event publishing with %1 displays added")
              .arg(added));
   LogInfo(QString("%1 displays total")
              .arg(event.Displays.size()));
   emit DisplayInfoModified(event);
}
