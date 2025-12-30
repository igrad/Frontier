#include "EnterpriseSettingsView.h"

using namespace Enterprise;

EnterpriseSettingsView::EnterpriseSettingsView(QWidget* parent)
   : QTableView(parent)
{
}

void EnterpriseSettingsView::HandleSettingsPopulated()
{
   resizeColumnsToContents();
}
