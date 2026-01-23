#include "EnterpriseHardwareTab.h"

#include "EnterpriseMonitorWidget.h"
#include "EnterpriseMonitorWindow.h"

using namespace Enterprise;

EnterpriseHardwareTab::EnterpriseHardwareTab()
   : QWidget(nullptr)
{
   BuildUI();
}

void EnterpriseHardwareTab::PublishInitialDisplayInfo()
{
   Monitor1Btn->PublishInitialDisplayInfo();
}

void EnterpriseHardwareTab::BuildUI()
{
   Layout = new QVBoxLayout(this);
   MonitorBtnLayout = new QHBoxLayout(this);
   Monitor1Btn = new EnterpriseMonitorWidget(this);
   Monitor2Btn = new EnterpriseMonitorWidget(this);
   Monitor3Btn = new EnterpriseMonitorWidget(this);
   Monitor4Btn = new EnterpriseMonitorWidget(this);
   MonitorBtnLayout->addWidget(Monitor1Btn);
   MonitorBtnLayout->addWidget(Monitor2Btn);
   MonitorBtnLayout->addWidget(Monitor3Btn);
   MonitorBtnLayout->addWidget(Monitor4Btn);
   connect(Monitor1Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseHardwareTab::DisplayInfoModified);
   connect(Monitor2Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseHardwareTab::DisplayInfoModified);
   connect(Monitor3Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseHardwareTab::DisplayInfoModified);
   connect(Monitor4Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseHardwareTab::DisplayInfoModified);
   Layout->addLayout(MonitorBtnLayout);
   this->setLayout(Layout);
}
