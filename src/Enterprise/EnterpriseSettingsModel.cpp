#include "EnterpriseSettingsModel.h"

#include <iostream>

using namespace Enterprise;
using namespace Settings;

namespace
{
   constexpr const int NUM_COLS = 2;
}

EnterpriseSettingsModel::EnterpriseSettingsModel(SettingsClientInterface* settingsClient,
                                                 QObject* parent)
   : SettingsClient(settingsClient)
   , Data()
{
   setParent(parent);
}

int EnterpriseSettingsModel::rowCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent);
   return Data.count();
}

int EnterpriseSettingsModel::columnCount(const QModelIndex& parent) const
{
   Q_UNUSED(parent);
   return NUM_COLS;
}

QVariant EnterpriseSettingsModel::data(const QModelIndex& index, int role) const
{
   QVariant val;
   const int rowIndex = index.row();
   const int colIndex = index.column();
   if(rowIndex < Data.count() &&
       colIndex < NUM_COLS)
   {
      const RowData row = Data.at(index.row());
      val = row.GetValue(index.column(),
                         static_cast<Qt::ItemDataRole>(role));
   }
   else
   {
      val = QVariant();
   }

   return val;
}

void EnterpriseSettingsModel::HandleSettingChanged(Setting setting,
                                                   const QVariant& value)
{
   std::cout << "Enterprise - HandleSettingChanged " << ToString(setting).toStdString() << std::endl;
   auto iter = std::find_if(Data.begin(), Data.end(),
                       [&](const RowData& data){
                          return data.TheSetting == setting;
   });

   if(Data.end() == iter)
   {
      std::cout << "Enterprise - " << ToString(setting).toStdString() << " not found, appending" << std::endl;
      RowData data;
      data.TheSetting = setting;
      data.SetValue(0, ToString(setting));
      data.SetValue(1, value);
      Data.insert(GetIndexToInsertSetting(setting), data);
   }
   else
   {
      std::cout << "Enterprise - found, updating" << std::endl;
      // TODO: In the future, make a cell change color for a bit
      // after the value changes.
      RowData& data = *iter;
      data.SetValue(1, value);
   }

   emit dataChanged(index(0, 0), index(rowCount(), NUM_COLS));
}

void EnterpriseSettingsModel::HandleDataAccessThreadStarted()
{
   std::cout << "Enterprise - SettingsModel subscribed to all signals" << std::endl;
   SettingsClient->SubscribeToAllSettings(this);
}

int EnterpriseSettingsModel::GetIndexToInsertSetting(Setting setting) const
{
   int retVal = 0;

   const QString str = ToString(setting);

   for(int iter = 0; iter < Data.count(); ++iter)
   {
      if(str < ToString(Data[iter].TheSetting))
      {
         retVal = iter;
         break;
      }
   }

   return retVal;
}
