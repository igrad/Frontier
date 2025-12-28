#pragma once

#include "EnterpriseService.h"

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
      EnterpriseWindow(Settings::SettingsClientInterface* settingsClient,
                       QWidget* parent = nullptr);
      ~EnterpriseWindow() = default;

   signals:
      void Resume();
      void Suspend();
      void MemoryDbSelected();
      void DiskDbSelected();
      void RetainAndRestoreStateChanged(bool active);

   private slots:
      void HandleDatabaseSourceComboBoxSelection(const QString& str);
      void HandleRetainAndRestoreCheckBoxCheck(bool checked);

   private:
      void BuildUI();

      EnterpriseSettingsModel* SettingsModel;

      QVBoxLayout* Layout;
      QHBoxLayout* SuspendControlsLayout;
      QPushButton* ResumeBtn;
      QPushButton* SuspendBtn;
      QHBoxLayout* DbControlsLayout;
      QComboBox* DatabaseSourceComboBox;
      QCheckBox* RetainAndRestoreCheckBox;
      EnterpriseSettingsView* SettingsView;

      bool RetainAndRestore;
   };
}
