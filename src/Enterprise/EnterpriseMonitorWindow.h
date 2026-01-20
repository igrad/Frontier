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

   signals:
      void DisplayInfoModified(const DisplayID& id, const DisplayInfo& info);

   private slots:
      void HandleConfirmPressed();
      void HandleCancelPressed();

   private:
      static QMap<uint8_t, DisplayInfo> DisplaysInfo;

      void CreateUI();
      void PopulateUI();
      DisplayInfo CompileDisplayInfoFromInputs();

      int ActiveDisplayNum;
      QVBoxLayout* Layout;
      QHBoxLayout* NumberAndNameLayout;
      QLabel* MonitorNumberLabel;
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
