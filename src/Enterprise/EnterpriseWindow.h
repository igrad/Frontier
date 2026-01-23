#pragma once

#include "EnterpriseService.h"

#include <DisplayInfo.h>

#include <QVBoxLayout>
#include <QTabWidget>

namespace Enterprise
{
   class EnterpriseHardwareTab;
   class EnterpriseSettingsModel;
   class EnterpriseSettingsTab;
   class EnterpriseSettingsView;

   class EnterpriseWindow: public QWidget
   {
      Q_OBJECT

   public:
      explicit EnterpriseWindow(QWidget* parent = nullptr);
      ~EnterpriseWindow() = default;

      void SetSettingsClient(Settings::SettingsClientInterface* settingsClient);

   signals:
      void Resume();
      void Suspend();
      void RetainAndRestoreStateChanged(bool active);
      void DatabaseStarted();
      void DisplayInfoModified(const DisplayConfigEvent& event);

   public slots:
      void HandleFrontierStarted();

   private slots:
      void HandleResume();
      void HandleDisplayInfoModified(const DisplayConfigEvent& event);

   private:
      void BuildUI();

      EnterpriseSettingsModel* SettingsModel;

      QVBoxLayout* Layout;
      QTabWidget* MasterTabWidget;

      EnterpriseSettingsTab* SettingsTab;

      EnterpriseHardwareTab* HardwareTab;

      bool UseRAMDbs;
      bool RetainAndRestore;

      DisplayConfigEvent LatestDisplayConfig;
   };
}
