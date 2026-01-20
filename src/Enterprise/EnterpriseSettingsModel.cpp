#include "EnterpriseSettingsModel.h"

#include <QFont>

using namespace Enterprise;
using namespace Settings;

namespace
{
   constexpr const int NUM_COLS = 2;
}

EnterpriseSettingsModel::EnterpriseSettingsModel(QObject* parent)
   : SettingsClient(nullptr)
   , Data()
   , DatabaseStarted(false)
{
   setParent(parent);
   PopulateSettings();
}

void EnterpriseSettingsModel::SetSettingsClient(Settings::SettingsClientInterface* settingsClient)
{
   SettingsClient = settingsClient;
   SettingsClient->SubscribeToAllSystemSettings(this);
   DatabaseStarted = true;
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


QVariant EnterpriseSettingsModel::headerData(int section,
                                             Qt::Orientation orientation,
                                             int role) const
{
   QVariant retVal;

   if(Qt::Horizontal == orientation)
   {
      if(Qt::FontRole == role)
      {
         QFont font;
         font.setBold(true);
         retVal = font;
      }
      else if(Qt::DisplayRole == role)
      {
         if(section == 0)
         {
            retVal = QString("Setting");
         }
         else if(section == 1)
         {
            retVal = QString("Value (as string)");
         }
      }
   }

   return retVal;
}


Qt::ItemFlags EnterpriseSettingsModel::flags(const QModelIndex& index) const
{
   Qt::ItemFlags flags;
   if(!index.isValid())
   {
      flags = Qt::NoItemFlags;
   }
   else
   {
      flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

      if(DatabaseStarted && (index.column() > 0))
      {
         flags |= Qt::ItemIsEditable;
      }
   }

   return flags;
}


bool EnterpriseSettingsModel::setData(const QModelIndex& index,
                                      const QVariant& value,
                                      int role)
{
   bool retVal = false;

   if(Qt::EditRole == role)
   {
      const QModelIndex settingIndex = index.siblingAtColumn(0);
      const Settings::Setting setting = Settings::ToSetting(settingIndex.data().toString());
      retVal = WriteEditedSetting(setting, value);
   }

   return retVal;
}

void EnterpriseSettingsModel::HandleSystemSettingChanged(Setting setting,
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
      data.SetValue(2, value);
      Data.insert(GetIndexToInsertSetting(setting), data);
   }
   else
   {
      // TODO: In the future, make a cell change color for a bit
      // after the value changes.
      RowData& data = *iter;
      data.SetValue(2, value);
   }
}

void EnterpriseSettingsModel::HandleDisplaySettingChanged(Setting setting,
                                                          uint8_t display,
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
      data.Display = display;
      data.SetValue(0, ToString(setting));
      data.SetValue(1, display);
      data.SetValue(2, value);
      Data.insert(GetIndexToInsertSetting(setting), data);
   }
   else
   {
      // TODO: In the future, make a cell change color for a bit
      // after the value changes.
      RowData& data = *iter;
      data.SetValue(1, display);
      data.SetValue(2, value);
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

int EnterpriseSettingsModel::GetIndexOfSetting(Settings::Setting setting) const
{
   int retVal = -1;
   for(int iter = 0; iter < Data.count(); ++iter)
   {
      const RowData& data = Data[iter];
      if(data.TheSetting == setting)
      {
         retVal = iter;
         break;
      }
   }

   return retVal;
}

void EnterpriseSettingsModel::PopulateSettings()
{
   for(int iter = 1; iter < static_cast<int>(Settings::Setting::_All); ++iter)
   {
      const Settings::Setting setting = static_cast<Settings::Setting>(iter);
      HandleSystemSettingChanged(setting, QVariant());
   }

   emit SettingsPopulated();
}

bool EnterpriseSettingsModel::WriteEditedSetting(Setting setting, const QVariant& value)
{
   bool retVal = false;
   const int dataIndex = GetIndexOfSetting(setting);

   if(-1 < dataIndex)
   {
      Data[dataIndex].SetValue(1, value);
      retVal = SettingsClient->WriteSystemSettingValue(setting, value);
   }

   return retVal;
}
