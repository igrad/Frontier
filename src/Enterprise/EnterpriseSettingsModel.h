#pragma once

#include "RowData.h"

#include <QAbstractTableModel>

namespace Enterprise
{

   class EnterpriseSettingsModel: public QAbstractTableModel
   {
      Q_OBJECT

   public:
      EnterpriseSettingsModel(Settings::SettingsClientInterface* settingsClient,
                              QObject* parent = nullptr);
      ~EnterpriseSettingsModel() = default;

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      int columnCount(const QModelIndex& parent = QModelIndex()) const override;
      QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

   public slots:
      void HandleSettingChanged(Settings::Setting setting,
                                const QVariant& value);

   private:
      int GetIndexToInsertSetting(Settings::Setting setting) const;

      QList<RowData> Data;
   };
}
