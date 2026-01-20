#include "EnterpriseMonitorWindow.h"

using namespace Enterprise;

QMap<uint8_t, DisplayInfo> EnterpriseMonitorWindow::DisplaysInfo;

EnterpriseMonitorWindow::EnterpriseMonitorWindow(int displayNum, QWidget* parent)
   : QWidget(parent)
   , ActiveDisplayNum(displayNum)
{
   setAttribute(Qt::WA_DeleteOnClose);

   CreateUI();
   PopulateUI();

   show();
}

void EnterpriseMonitorWindow::HandleConfirmPressed()
{
   const DisplayInfo& info = CompileDisplayInfoFromInputs();

   if(DisplaysInfo[ActiveDisplayNum] != info)
   {
      emit DisplayInfoModified(info.ID, info);
   }

   close();
}


void EnterpriseMonitorWindow::CreateUI()
{
   Layout = new QVBoxLayout(this);

   NumberAndNameLayout = new QHBoxLayout(this);
   MonitorNumberLabel = new QLabel(this);
   DisplayNameLineEdit = new QLineEdit(this);
   NumberAndNameLayout->addWidget(MonitorNumberLabel);
   NumberAndNameLayout->addWidget(DisplayNameLineEdit);
   Layout->addLayout(NumberAndNameLayout);

   DisplayRectLayout = new QHBoxLayout(this);
   DisplayWidthLabel = new QLabel(this);
   DisplayWidthLabel->setText("Width");
   DisplayWidthLineEdit = new QLineEdit(this);
   DisplayWidthLineEdit->setInputMask("9999");
   DisplayHeightLabel = new QLabel(this);
   DisplayHeightLabel->setText("Height");
   DisplayHeightLineEdit = new QLineEdit(this);
   DisplayHeightLineEdit->setInputMask("9999");
   DisplayRectLayout->addWidget(DisplayWidthLabel);
   DisplayRectLayout->addWidget(DisplayWidthLineEdit);
   DisplayRectLayout->addWidget(DisplayHeightLabel);
   DisplayRectLayout->addWidget(DisplayHeightLineEdit);
   Layout->addLayout(DisplayRectLayout);

   DisplayDPILayout = new QHBoxLayout(this);
   XDPILabel = new QLabel(this);
   XDPILabel->setText("X DPI");
   XDPILineEdit = new QLineEdit(this);
   XDPILineEdit->setInputMask("999");
   YDPILabel = new QLabel(this);
   YDPILabel->setText("Y DPI");
   YDPILineEdit = new QLineEdit(this);
   YDPILineEdit->setInputMask("999");
   DisplayDPILayout->addWidget(XDPILabel);
   DisplayDPILayout->addWidget(XDPILineEdit);
   DisplayDPILayout->addWidget(YDPILabel);
   DisplayDPILayout->addWidget(YDPILineEdit);
   Layout->addLayout(DisplayDPILayout);

   ButtonsLayout = new QHBoxLayout(this);
   ConfirmBtn = new QPushButton(this);
   ConfirmBtn->setText("Confirm");
   connect(ConfirmBtn, &QPushButton::clicked,
           this, &EnterpriseMonitorWindow::HandleConfirmPressed);
   CancelBtn = new QPushButton(this);
   CancelBtn->setText("Cancel");
   connect(CancelBtn, &QPushButton::clicked,
           this, &QPushButton::close);
   ButtonsLayout->addWidget(ConfirmBtn);
   ButtonsLayout->addWidget(CancelBtn);
   Layout->addLayout(ButtonsLayout);

   setLayout(Layout);
}

void EnterpriseMonitorWindow::PopulateUI()
{
   const DisplayInfo& info = DisplaysInfo[ActiveDisplayNum];
   MonitorNumberLabel->setText(QString::number(info.Number));
   DisplayNameLineEdit->setText(info.DisplayName);
   DisplayWidthLabel->setText(QString::number(info.Rect.width()));
   DisplayHeightLabel->setText(QString::number(info.Rect.height()));
   XDPILineEdit->setText(QString::number(info.XDPI));
   YDPILineEdit->setText(QString::number(info.YDPI));
}

DisplayInfo EnterpriseMonitorWindow::CompileDisplayInfoFromInputs()
{
   DisplayInfo info = DisplaysInfo[ActiveDisplayNum];

   info.DisplayName = DisplayNameLineEdit->text();
   info.Number = ActiveDisplayNum;
   info.Rect.setRect(
      0,
      0,
      DisplayWidthLineEdit->text().toInt(),
      DisplayHeightLineEdit->text().toInt());
   info.XDPI = XDPILineEdit->text().toInt();
   info.YDPI = YDPILineEdit->text().toInt();

   return info;
}
