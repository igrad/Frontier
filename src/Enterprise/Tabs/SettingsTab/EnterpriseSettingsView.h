#pragma once

#include <QTableView>

namespace Enterprise
{
   class EnterpriseSettingsView: public QTableView
   {
      Q_OBJECT

   public:
      EnterpriseSettingsView(QWidget* parent = nullptr);
      ~EnterpriseSettingsView() = default;

   public slots:
      void HandleSettingsPopulated();
   };
}
