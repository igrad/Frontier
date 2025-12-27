#include "EnterpriseWindow.h"

using namespace Enterprise;

EnterpriseWindow::EnterpriseWindow(QWidget* parent)
   : QWidget(parent)
   , Layout(nullptr)
   , ResumeBtn(nullptr)
   , SuspendBtn(nullptr)
   , DatabaseSourceComboBox(nullptr)
   , RetainAndRestoreCheckBox(nullptr)
{
   BuildUI();
   show();
}

void EnterpriseWindow::ConnectToService(EnterpriseService* service)
{
   Q_UNUSED(service);
}

void EnterpriseWindow::BuildUI()
{
   Layout = new QVBoxLayout(this);

   ResumeBtn = new QPushButton("Resume", this);
   SuspendBtn = new QPushButton("Suspend", this);

   DatabaseSourceComboBox = new QComboBox(this);
   DatabaseSourceComboBox->addItem(":memory:");
   DatabaseSourceComboBox->addItem("Disk");
   setLayout(Layout);
}
