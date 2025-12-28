#include "EnterpriseWindow.h"

#include "EnterpriseSettingsModel.h"
#include "EnterpriseSettingsView.h"

using namespace Enterprise;

namespace
{
   constexpr const char* const MEMORY_STR = ":memory:";
   constexpr const char* const DISK_STR = "Disk";
}

EnterpriseWindow::EnterpriseWindow(Settings::SettingsClientInterface* settingsClient,
                                   QWidget* parent)
   : QWidget(parent)
   , SettingsModel(new EnterpriseSettingsModel(settingsClient, this))
   , Layout(nullptr)
   , SuspendControlsLayout(nullptr)
   , ResumeBtn(nullptr)
   , SuspendBtn(nullptr)
   , DbControlsLayout(nullptr)
   , DatabaseSourceComboBox(nullptr)
   , RetainAndRestoreCheckBox(nullptr)
   , SettingsView(nullptr)
   , RetainAndRestore(false)
{
   BuildUI();

   connect(ResumeBtn, &QPushButton::released,
           this, &EnterpriseWindow::Resume);
   connect(SuspendBtn, &QPushButton::released,
           this, &EnterpriseWindow::Suspend);
   connect(DatabaseSourceComboBox, &QComboBox::currentTextChanged,
           this, &EnterpriseWindow::HandleDatabaseSourceComboBoxSelection);
   connect(RetainAndRestoreCheckBox, &QCheckBox::checkStateChanged,
           this, &EnterpriseWindow::HandleRetainAndRestoreCheckBoxCheck);
   show();
}

void EnterpriseWindow::HandleFrontierStarted()
{
   RetainAndRestoreCheckBox->setDisabled(true);
   DatabaseSourceComboBox->setDisabled(true);
}

void EnterpriseWindow::HandleDatabaseSourceComboBoxSelection(const QString& str)
{
   if(MEMORY_STR == str)
   {
      emit MemoryDbSelected();
      if(RetainAndRestore)
      {
         RetainAndRestoreCheckBox->setCheckState(Qt::CheckState::Unchecked);
         RetainAndRestoreCheckBox->setDisabled(true);
         HandleRetainAndRestoreCheckBoxCheck(false);// maybe this triggers itself?
      }
   }
   else if(DISK_STR == str)
   {
      emit DiskDbSelected();
      RetainAndRestoreCheckBox->setDisabled(false);
   }
}

void EnterpriseWindow::HandleRetainAndRestoreCheckBoxCheck(bool checked)
{
   if(checked != RetainAndRestore)
   {
      RetainAndRestore = checked;
      emit RetainAndRestoreStateChanged(checked);
   }
}

void EnterpriseWindow::BuildUI()
{
   Layout = new QVBoxLayout(this);

   SuspendControlsLayout = new QHBoxLayout(this);
   ResumeBtn = new QPushButton("Resume", this);
   SuspendBtn = new QPushButton("Suspend", this);
   SuspendControlsLayout->addWidget(ResumeBtn);
   SuspendControlsLayout->addWidget(SuspendBtn);
   Layout->addLayout(SuspendControlsLayout);

   DbControlsLayout = new QHBoxLayout(this);
   DatabaseSourceComboBox = new QComboBox(this);
   DatabaseSourceComboBox->addItem(MEMORY_STR);
   DatabaseSourceComboBox->addItem(DISK_STR);
   RetainAndRestoreCheckBox = new QCheckBox("Retain & Restore", this);
   DbControlsLayout->addWidget(DatabaseSourceComboBox);
   DbControlsLayout->addWidget(RetainAndRestoreCheckBox);
   Layout->addLayout(DbControlsLayout);

   SettingsView = new EnterpriseSettingsView(this);
   SettingsView->setModel(SettingsModel);
   Layout->addWidget(SettingsView);

   setLayout(Layout);
}
