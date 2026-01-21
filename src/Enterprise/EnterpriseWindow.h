#pragma once

#include "EnterpriseService.h"
#include "Widgets/EnterpriseMonitorWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Enterprise
{
   class EnterpriseSettingsView;
   class EnterpriseSettingsModel;

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
      void DataAccessThreadStarted();

   public slots:
      void HandleFrontierStarted();

   private slots:
      void HandleResumeBtnReleased();
      void HandleSuspendBtnReleased();
      void HandleDatabaseSourceComboBoxSelection(const QString& str);
      void HandleRetainAndRestoreCheckBoxCheck(bool checked);
      void HandleStartDatabaseBtnReleased();

   private:
      void BuildUI();

      EnterpriseSettingsModel* SettingsModel;

      QVBoxLayout* Layout;
      QTabWidget* MasterTabWidget;

      QWidget* Tab1;
      QVBoxLayout* Tab1Layout;
      QHBoxLayout* SuspendControlsLayout;
      QPushButton* ResumeBtn;
      QPushButton* SuspendBtn;
      QVBoxLayout* DbControlsLayout;
      QHBoxLayout* DbControlsUpperLayout;
      QComboBox* DatabaseSourceComboBox;
      QCheckBox* RetainAndRestoreCheckBox;
      QPushButton* StartDatabaseBtn;
      QHBoxLayout* DbControlsLowerLayout;
      QLineEdit* DatabaseUploadTextEdit;
      QPushButton* DatabaseUploadBtn;
      EnterpriseSettingsView* SettingsView;

      QWidget* Tab2;
      QVBoxLayout* Tab2Layout;
      // Also need to connect EnterpriseMonitorWindow::DisplayInfoModified signal to the service
      // and need to set up the Win32APIWrapperFake connection from the service, then connect the 2
      QHBoxLayout* MonitorBtnLayout;
      EnterpriseMonitorWidget* Monitor1Btn;
      EnterpriseMonitorWidget* Monitor2Btn;
      EnterpriseMonitorWidget* Monitor3Btn;
      EnterpriseMonitorWidget* Monitor4Btn;

      bool UseRAMDbs;
      bool RetainAndRestore;
   };
}
