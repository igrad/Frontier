#include "EnterpriseWindow.h"

#include "EnterpriseSettingsModel.h"
#include "EnterpriseSettingsView.h"

#include <QLineEdit>

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
   , DbControlsUpperLayout(nullptr)
   , DatabaseSourceComboBox(nullptr)
   , RetainAndRestoreCheckBox(nullptr)	// TODO: Unimplemented currently
   , StartDatabaseBtn(nullptr)
   , DbControlsLowerLayout(nullptr)
   , DatabaseUploadTextEdit(nullptr)
   , DatabaseUploadBtn(nullptr)
   , SettingsView(nullptr)
   , RetainAndRestore(false)
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
   show();
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

   emit RetainAndRestoreStateChanged(RetainAndRestore);
   emit UseRAMDatabases(UseRAMDbs);
   emit DatabaseStarted();

   ResumeBtn->setDisabled(false);
}

void EnterpriseWindow::BuildUI()
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
   DatabaseSourceComboBox->setCurrentText("");
   RetainAndRestoreCheckBox = new QCheckBox("Retain & Restore", this);
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
   SettingsView->setModel(SettingsModel);
   Layout->addWidget(SettingsView);

   setLayout(Layout);
}
