#include "EnterpriseWindow.h"

#include "Settings/EnterpriseSettingsModel.h"
#include "Widgets/EnterpriseSettingsView.h"

#include <SettingsService.h>

#include <QLineEdit>

using namespace Enterprise;

namespace
{
   constexpr const char* const MEMORY_STR = ":memory:";
   constexpr const char* const DISK_STR = "Disk";
}

EnterpriseWindow::EnterpriseWindow(QWidget* parent)
   : QWidget(parent)
   , SettingsModel(new EnterpriseSettingsModel(this))
{
   BuildUI();

   connect(ResumeBtn, &QPushButton::released,
           this, &EnterpriseWindow::HandleResumeBtnReleased);
   connect(SuspendBtn, &QPushButton::released,
           this, &EnterpriseWindow::HandleSuspendBtnReleased);
   connect(DatabaseSourceComboBox, &QComboBox::currentTextChanged,
           this, &EnterpriseWindow::HandleDatabaseSourceComboBoxSelection);
   connect(RetainAndRestoreCheckBox, &QCheckBox::checkStateChanged,
           this, &EnterpriseWindow::HandleRetainAndRestoreCheckBoxCheck);
   connect(StartDatabaseBtn, &QPushButton::released,
           this, &EnterpriseWindow::HandleStartDatabaseBtnReleased);

   // Fetches the initial display info to prep it to send to Frontier
   Monitor1Btn->PublishInitialDisplayInfo();
   setWindowTitle("Enterprise");
   show();
}

void EnterpriseWindow::SetSettingsClient(Settings::SettingsClientInterface* settingsClient)
{
   SettingsModel->SetSettingsClient(settingsClient);
}

void EnterpriseWindow::HandleFrontierStarted()
{
   RetainAndRestoreCheckBox->setDisabled(true);
   DatabaseSourceComboBox->setDisabled(true);
}

void EnterpriseWindow::HandleResumeBtnReleased()
{
   ResumeBtn->setDisabled(true);
   SuspendBtn->setDisabled(false);

   emit DisplayInfoModified(LatestDisplayConfig);
   emit Resume();
}

void EnterpriseWindow::HandleSuspendBtnReleased()
{
   ResumeBtn->setDisabled(false);
   SuspendBtn->setDisabled(true);
   emit Suspend();
}

void EnterpriseWindow::HandleDatabaseSourceComboBoxSelection(const QString& str)
{
   if(MEMORY_STR == str)
   {
      UseRAMDbs = true;
      if(RetainAndRestore)
      {
         RetainAndRestoreCheckBox->setCheckState(Qt::CheckState::Unchecked);
         HandleRetainAndRestoreCheckBoxCheck(false);
         RetainAndRestoreCheckBox->setDisabled(true);
      }
   }
   else if(DISK_STR == str)
   {
      UseRAMDbs = false;
      RetainAndRestoreCheckBox->setDisabled(false);
   }
}

void EnterpriseWindow::HandleRetainAndRestoreCheckBoxCheck(bool checked)
{
   if(checked != RetainAndRestore)
   {
      RetainAndRestore = checked;
   }
}

void EnterpriseWindow::HandleStartDatabaseBtnReleased()
{
   RetainAndRestoreCheckBox->setDisabled(true);
   DatabaseSourceComboBox->setDisabled(true);
   StartDatabaseBtn->setDisabled(true);

   // I know it's hacky, but it's only for a debug tool so I don't care.
   Settings::SettingsService::UseRAMDatabases = UseRAMDbs;
   emit RetainAndRestoreStateChanged(RetainAndRestore);
   emit DatabaseStarted();

   ResumeBtn->setDisabled(false);
}


void EnterpriseWindow::HandleDisplayInfoModified(const DisplayConfigEvent& event)
{
   LatestDisplayConfig = event;
   emit DisplayInfoModified(event);
}

void EnterpriseWindow::BuildUI()
{
   Layout = new QVBoxLayout(this);
   MasterTabWidget = new QTabWidget(this);

   // Tab 1
   Tab1 = new QWidget(); // Qt docs say not to make the tabs parented!
   Tab1Layout = new QVBoxLayout(Tab1);
   SuspendControlsLayout = new QHBoxLayout(Tab1);
   ResumeBtn = new QPushButton("Resume", Tab1);
   ResumeBtn->setDisabled(true);
   SuspendBtn = new QPushButton("Suspend", Tab1);
   SuspendBtn->setDisabled(true);
   SuspendControlsLayout->addWidget(ResumeBtn);
   SuspendControlsLayout->addWidget(SuspendBtn);
   Tab1Layout->addLayout(SuspendControlsLayout);

   DbControlsLayout = new QVBoxLayout(Tab1);
   DbControlsUpperLayout = new QHBoxLayout(Tab1);
   DatabaseSourceComboBox = new QComboBox(Tab1);
   DatabaseSourceComboBox->addItem(MEMORY_STR);
   DatabaseSourceComboBox->addItem(DISK_STR);
   RetainAndRestoreCheckBox = new QCheckBox("Retain+Restore", Tab1);
   RetainAndRestoreCheckBox->setDisabled(true);
   StartDatabaseBtn = new QPushButton("Start database", Tab1);
   DbControlsUpperLayout->addWidget(DatabaseSourceComboBox);
   DbControlsUpperLayout->addWidget(RetainAndRestoreCheckBox);
   DbControlsUpperLayout->addWidget(StartDatabaseBtn);
   DbControlsLayout->addLayout(DbControlsUpperLayout);
   DbControlsLowerLayout = new QHBoxLayout(Tab1);
   DatabaseUploadTextEdit = new QLineEdit(Tab1);
   DatabaseUploadTextEdit->setPlaceholderText("Upload Settings.db file");
   DatabaseUploadTextEdit->setDisabled(true); // Disabled
   DatabaseUploadBtn = new QPushButton("Select", Tab1);
   DatabaseUploadBtn->setDisabled(true); // Disabled
   DbControlsLowerLayout->addWidget(DatabaseUploadTextEdit);
   DbControlsLowerLayout->addWidget(DatabaseUploadBtn);
   DbControlsLayout->addLayout(DbControlsLowerLayout);
   Tab1Layout->addLayout(DbControlsLayout);

   SettingsView = new EnterpriseSettingsView(Tab1);
   connect(SettingsModel, &EnterpriseSettingsModel::SettingsPopulated,
           SettingsView, &EnterpriseSettingsView::HandleSettingsPopulated,
           Qt::UniqueConnection);
   SettingsView->setModel(SettingsModel);
   Tab1Layout->addWidget(SettingsView);

   Tab1->setLayout(Tab1Layout);

   MasterTabWidget->addTab(Tab1, "Settings");

   // Tab 2
   Tab2 = new QWidget();
   Tab2Layout = new QVBoxLayout(Tab2);
   MonitorBtnLayout = new QHBoxLayout(Tab2);
   Monitor1Btn = new EnterpriseMonitorWidget(Tab2);
   Monitor2Btn = new EnterpriseMonitorWidget(Tab2);
   Monitor3Btn = new EnterpriseMonitorWidget(Tab2);
   Monitor4Btn = new EnterpriseMonitorWidget(Tab2);
   MonitorBtnLayout->addWidget(Monitor1Btn);
   MonitorBtnLayout->addWidget(Monitor2Btn);
   MonitorBtnLayout->addWidget(Monitor3Btn);
   MonitorBtnLayout->addWidget(Monitor4Btn);
   connect(Monitor1Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseWindow::HandleDisplayInfoModified);
   connect(Monitor2Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseWindow::HandleDisplayInfoModified);
   connect(Monitor3Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseWindow::HandleDisplayInfoModified);
   connect(Monitor4Btn, &EnterpriseMonitorWidget::DisplayInfoModified,
           this, &EnterpriseWindow::HandleDisplayInfoModified);
   Tab2Layout->addLayout(MonitorBtnLayout);
   Tab2->setLayout(Tab2Layout);

   MasterTabWidget->addTab(Tab2, "Hardware");

   Layout->addWidget(MasterTabWidget);
}
