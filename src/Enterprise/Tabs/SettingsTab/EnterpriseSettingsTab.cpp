#include "EnterpriseSettingsTab.h"

#include "SettingsModel/EnterpriseSettingsModel.h"
#include "EnterpriseSettingsView.h"

#include <SettingsService.h>

#include <QLineEdit>

using namespace Enterprise;

namespace
{
   constexpr const char* const MEMORY_STR = ":memory:";
   constexpr const char* const DISK_STR = "Disk";
}

EnterpriseSettingsTab::EnterpriseSettingsTab(EnterpriseSettingsModel* model)
   : QWidget(nullptr)
   , Model(model)
{
   BuildUI();

   connect(ResumeBtn, &QPushButton::released,
           this, &EnterpriseSettingsTab::HandleResumeBtnReleased);
   connect(SuspendBtn, &QPushButton::released,
           this, &EnterpriseSettingsTab::HandleSuspendBtnReleased);
   connect(DatabaseSourceComboBox, &QComboBox::currentTextChanged,
           this, &EnterpriseSettingsTab::HandleDatabaseSourceComboBoxSelection);
   connect(RetainAndRestoreCheckBox, &QCheckBox::checkStateChanged,
           this, &EnterpriseSettingsTab::HandleRetainAndRestoreCheckBoxCheck);
   connect(StartDatabaseBtn, &QPushButton::released,
           this, &EnterpriseSettingsTab::HandleStartDatabaseBtnReleased);
}

void EnterpriseSettingsTab::HandleFrontierStarted()
{
   RetainAndRestoreCheckBox->setDisabled(true);
   DatabaseSourceComboBox->setDisabled(true);
}

void EnterpriseSettingsTab::HandleResumeBtnReleased()
{
   ResumeBtn->setDisabled(true);
   SuspendBtn->setDisabled(false);

   emit Resume();
}

void EnterpriseSettingsTab::HandleSuspendBtnReleased()
{
   ResumeBtn->setDisabled(false);
   SuspendBtn->setDisabled(true);
   emit Suspend();
}

void EnterpriseSettingsTab::HandleDatabaseSourceComboBoxSelection(const QString& str)
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

void EnterpriseSettingsTab::HandleRetainAndRestoreCheckBoxCheck(bool checked)
{
   if(checked != RetainAndRestore)
   {
      RetainAndRestore = checked;
   }
}

void EnterpriseSettingsTab::HandleStartDatabaseBtnReleased()
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

void EnterpriseSettingsTab::BuildUI()
{
   Layout = new QVBoxLayout(this);
   SuspendControlsLayout = new QHBoxLayout(this);
   ResumeBtn = new QPushButton("Resume", this);
   ResumeBtn->setDisabled(true);
   SuspendBtn = new QPushButton("Suspend", this);
   SuspendBtn->setDisabled(true);
   SuspendControlsLayout->addWidget(ResumeBtn);
   SuspendControlsLayout->addWidget(SuspendBtn);
   Layout->addLayout(SuspendControlsLayout);

   DbControlsLayout = new QVBoxLayout(this);
   DbControlsUpperLayout = new QHBoxLayout(this);
   DatabaseSourceComboBox = new QComboBox(this);
   DatabaseSourceComboBox->addItem(MEMORY_STR);
   DatabaseSourceComboBox->addItem(DISK_STR);
   RetainAndRestoreCheckBox = new QCheckBox("Retain+Restore", this);
   RetainAndRestoreCheckBox->setDisabled(true);
   StartDatabaseBtn = new QPushButton("Start database", this);
   DbControlsUpperLayout->addWidget(DatabaseSourceComboBox);
   DbControlsUpperLayout->addWidget(RetainAndRestoreCheckBox);
   DbControlsUpperLayout->addWidget(StartDatabaseBtn);
   DbControlsLayout->addLayout(DbControlsUpperLayout);
   DbControlsLowerLayout = new QHBoxLayout(this);
   DatabaseUploadTextEdit = new QLineEdit(this);
   DatabaseUploadTextEdit->setPlaceholderText("Upload Settings.db file");
   DatabaseUploadTextEdit->setDisabled(true); // Disabled
   DatabaseUploadBtn = new QPushButton("Select", this);
   DatabaseUploadBtn->setDisabled(true); // Disabled
   DbControlsLowerLayout->addWidget(DatabaseUploadTextEdit);
   DbControlsLowerLayout->addWidget(DatabaseUploadBtn);
   DbControlsLayout->addLayout(DbControlsLowerLayout);
   Layout->addLayout(DbControlsLayout);

   SettingsView = new EnterpriseSettingsView(this);
   connect(Model, &EnterpriseSettingsModel::SettingsPopulated,
           SettingsView, &EnterpriseSettingsView::HandleSettingsPopulated,
           Qt::UniqueConnection);
   SettingsView->setModel(Model);
   Layout->addWidget(SettingsView);
}
