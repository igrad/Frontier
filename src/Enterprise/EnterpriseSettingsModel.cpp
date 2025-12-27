#include "EnterpriseSettingsModel.h"

using namespace Enterprise;
using namespace Settings;

EnterpriseSettingsModel::EnterpriseSettingsModel(QObject* parent)
   : Data()
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
   return 2;
}

QVariant EnterpriseSettingsModel::data(const QModelIndex& index, int role) const
{
   QVariant val;
   const int rowIndex = index.row();
   const int colIndex = index.column();
   if(rowIndex < Data.count() &&
       colIndex < 2)
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
   auto iter = std::find_if(Data.begin(), Data.end(),
                       [&](const RowData& data){
                          return data.TheSetting == setting;
   });

   if(Data.end() == iter)
   {
      RowData data;
      data.TheSetting = setting;
      data.SetValue(0, ToString(setting));
      data.SetValue(1, value);
      Data.insert(GetIndexToInsertSetting(setting), data);
   }
   else
   {
      // TODO: In the future, make a cell change color for a bit
      // after the value changes.
      RowData& data = *iter;
      data.SetValue(1, value);
   }
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
