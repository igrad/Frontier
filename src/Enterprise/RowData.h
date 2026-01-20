#pragma once

#include <SettingsService/SettingsClientInterface.h>

namespace Enterprise
{
   struct RowData
   {
      QVariant GetValue(int cell,
              Qt::ItemDataRole role = Qt::DisplayRole) const
      {
         QVariant retVal;
         if((0 <= cell) && (3 > cell))
         {
            retVal = Data[cell].value(role);
         }

         return retVal;
      }

      void SetValue(int cell,
          const QVariant& value,
          Qt::ItemDataRole role = Qt::DisplayRole)
      {
         if((0 <= cell) && (3 > cell))
         {
            Data[cell][role] = value;
         }
      }

      Settings::Setting TheSetting = Settings::Setting::None;
      uint8_t Display = 0;
      QHash<Qt::ItemDataRole, QVariant> Data[3];
   };
}
