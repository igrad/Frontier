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
      explicit EnterpriseWindow(QWidget* parent = nullptr);
      ~EnterpriseWindow() = default;

      void ConnectToService(EnterpriseService* service);

   private slots:
      void HandleResumeBtnReleased();
      void HandleSuspendBtnReleased();
      void HandleDatabaseSourceComboBoxSelection(const QString& str);
      void HandleRetainAndRestoreCheckBoxCheck(bool checked);

   private:
      void BuildUI();

      EnterpriseSettingsModel* SettingsModel;

      QVBoxLayout* Layout;
      QPushButton* ResumeBtn;
      QPushButton* SuspendBtn;
      QComboBox* DatabaseSourceComboBox;
      QCheckBox* RetainAndRestoreCheckBox;
      EnterpriseSettingsView* SettingsView;
   };
}
