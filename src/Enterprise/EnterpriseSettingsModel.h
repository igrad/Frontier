#pragma once

#include <SettingsService/SettingsClientInterface.h>

#include <QAbstractTableModel>

namespace Enterprise
{
   class EnterpriseSettingsModel: public QAbstractTableModel
   {
      Q_OBJECT

   public:
      explicit EnterpriseSettingsModel(QObject* parent = nullptr);
      ~EnterpriseSettingsModel() = default;

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      int columnCount(const QModelIndex& parent = QModelIndex()) const override;
      QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

   public slots:
      void HandleSettingChanged(Settings::Setting setting,
                                const QVariant& value);

   private:
      struct RowData
      {
         QVariant GetValue(int cell,
                           Qt::ItemDataRole role = Qt::DisplayRole) const;
         void SetValue(int cell,
                       const QVariant& value,
                       Qt::ItemDataRole role = Qt::DisplayRole);

         Settings::Setting TheSetting = Settings::Setting::None;
         QHash<Qt::ItemDataRole, QVariant> Data[2];
      };

      int GetIndexToInsertSetting(Settings::Setting setting) const;

      QList<RowData> Data;
   };
}
