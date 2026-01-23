#include "EnterpriseWindow.h"

#include <EnterpriseHardwareTab.h>
#include <EnterpriseSettingsTab.h>
#include <SettingsModel/EnterpriseSettingsModel.h>

#include <SettingsService.h>

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

   // Fetches the initial display info to prep it to send to Frontier
   HardwareTab->PublishInitialDisplayInfo();

   setWindowTitle("Enterprise");
   show();
}

void EnterpriseWindow::SetSettingsClient(Settings::SettingsClientInterface* settingsClient)
{
   SettingsModel->SetSettingsClient(settingsClient);
}

void EnterpriseWindow::HandleFrontierStarted()
{
   SettingsTab->HandleFrontierStarted();
}

void EnterpriseWindow::HandleResume()
{
   emit DisplayInfoModified(LatestDisplayConfig);
   emit Resume();
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
   SettingsTab = new EnterpriseSettingsTab(SettingsModel); // Qt docs say not to make the tabs parented!
   connect(SettingsTab, &EnterpriseSettingsTab::Resume,
           this, &EnterpriseWindow::HandleResume);
   connect(SettingsTab, &EnterpriseSettingsTab::Suspend,
           this, &EnterpriseWindow::Suspend);
   connect(SettingsTab, &EnterpriseSettingsTab::DatabaseStarted,
           this, &EnterpriseWindow::DatabaseStarted);

   MasterTabWidget->addTab(SettingsTab, "Settings");

   // Tab 2
   HardwareTab = new EnterpriseHardwareTab();
   connect(HardwareTab, &EnterpriseHardwareTab::DisplayInfoModified,
           this, &EnterpriseWindow::HandleDisplayInfoModified);
   MasterTabWidget->addTab(HardwareTab, "Hardware");

   Layout->addWidget(MasterTabWidget);
}
