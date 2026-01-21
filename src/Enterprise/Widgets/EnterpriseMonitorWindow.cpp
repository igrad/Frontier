#include "EnterpriseMonitorWindow.h"

#include <QCloseEvent>

using namespace Enterprise;

QMap<uint8_t, DisplayInfo> EnterpriseMonitorWindow::DisplaysInfo;
QMap<uint8_t, HMONITOR__> EnterpriseMonitorWindow::HMonitors;

EnterpriseMonitorWindow::EnterpriseMonitorWindow(int displayNum, QWidget* parent)
   : QWidget(parent)
   , DisplayNum(displayNum)
{
   setAttribute(Qt::WA_DeleteOnClose);

   setWindowTitle(QString ("Configuring Monitor %1").arg(displayNum));

   CreateUI();
   PopulateUI();

   show();
}

void EnterpriseMonitorWindow::HandleConfirmPressed()
{
   const DisplayInfo& info = CompileDisplayInfoFromInputs();

   if(DisplaysInfo[DisplayNum] != info)
   {
      DisplaysInfo[DisplayNum] = info;
      emit DisplayInfoModified();
   }

   close();
}

void EnterpriseMonitorWindow::closeEvent(QCloseEvent* event)
{
   emit Closed();
   event->accept();
}

void EnterpriseMonitorWindow::CreateUI()
{
   Layout = new QVBoxLayout(this);

   MonitorNumberLabel = new QLabel(this);
   Layout->addWidget(MonitorNumberLabel);

   NameLayout = new QHBoxLayout();
   DisplayNameLabel = new QLabel(this);
   DisplayNameLabel->setText("Display Name");
   DisplayNameLineEdit = new QLineEdit(this);
   NameLayout->addWidget(DisplayNameLabel);
   NameLayout->addWidget(DisplayNameLineEdit);
   Layout->addLayout(NameLayout);

   DisplayRectLayout = new QHBoxLayout();
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

   DisplayDPILayout = new QHBoxLayout();
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

   ButtonsLayout = new QHBoxLayout();
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
}

void EnterpriseMonitorWindow::PopulateUI()
{
   const DisplayInfo& info = DisplaysInfo[DisplayNum];
   MonitorNumberLabel->setText(QString("Monitor %1").arg(DisplayNum));
   DisplayNameLineEdit->setText(info.DisplayName);
   DisplayWidthLineEdit->setText(QString::number(info.Rect.width()));
   DisplayHeightLineEdit->setText(QString::number(info.Rect.height()));
   XDPILineEdit->setText(QString::number(info.XDPI));
   YDPILineEdit->setText(QString::number(info.YDPI));
}

DisplayInfo EnterpriseMonitorWindow::CompileDisplayInfoFromInputs()
{
   DisplayInfo info = DisplaysInfo[DisplayNum];

   info.DisplayName = DisplayNameLineEdit->text();
   info.Number = DisplayNum;
   info.Rect.setRect(
      0,
      0,
      DisplayWidthLineEdit->text().toInt(),
      DisplayHeightLineEdit->text().toInt());
   info.XDPI = XDPILineEdit->text().toInt();
   info.YDPI = YDPILineEdit->text().toInt();

   return info;
}

