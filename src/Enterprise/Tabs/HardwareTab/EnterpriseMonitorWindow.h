#pragma once

#include <DisplayInfo.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Enterprise
{
   class EnterpriseMonitorWindow: public QWidget
   {
      Q_OBJECT

   public:
      EnterpriseMonitorWindow(int displayNum, QWidget* parent = nullptr);
      ~EnterpriseMonitorWindow() = default;

      static QMap<unsigned int, DisplayInfo> DisplaysInfo;
      static QMap<unsigned int, HMONITOR__> HMonitors;

   signals:
      void DisplayInfoModified();
      void Closed();

   private slots:
      void HandleConfirmPressed();
      void closeEvent(QCloseEvent* event) override;

   private:
      void CreateUI();
      void PopulateUI();
      DisplayInfo CompileDisplayInfoFromInputs();

      int DisplayNum;
      QVBoxLayout* Layout;
      QLabel* MonitorNumberLabel;

      QHBoxLayout* NameLayout;
      QLabel* DisplayNameLabel;
      QLineEdit* DisplayNameLineEdit;

      QHBoxLayout* DisplayRectLayout;
      QLabel* DisplayWidthLabel;
      QLineEdit* DisplayWidthLineEdit;
      QLabel* DisplayHeightLabel;
      QLineEdit* DisplayHeightLineEdit;

      QHBoxLayout* DisplayDPILayout;
      QLabel* XDPILabel;
      QLabel* YDPILabel;
      QLineEdit* XDPILineEdit;
      QLineEdit* YDPILineEdit;

      QHBoxLayout* ButtonsLayout;
      QPushButton* ConfirmBtn;
      QPushButton* CancelBtn;
   };
}
