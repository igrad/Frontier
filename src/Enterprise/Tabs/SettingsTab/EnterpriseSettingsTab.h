#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Enterprise
{
   class EnterpriseSettingsView;
   class EnterpriseSettingsModel;

   class EnterpriseSettingsTab: public QWidget
   {
      Q_OBJECT

   public:
      explicit EnterpriseSettingsTab(EnterpriseSettingsModel* model);
      ~EnterpriseSettingsTab() = default;

   signals:
      void Resume();
      void Suspend();
      void RetainAndRestoreStateChanged(bool active);
      void DatabaseStarted();

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

      EnterpriseSettingsModel* Model;

      QVBoxLayout* Layout;
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

      bool UseRAMDbs;
      bool RetainAndRestore;
   };
}
