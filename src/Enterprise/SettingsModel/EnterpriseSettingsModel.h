#pragma once

#include "RowData.h"

#include <QAbstractTableModel>

namespace Enterprise
{

   class EnterpriseSettingsModel: public QAbstractTableModel
   {
      Q_OBJECT

   public:
      explicit EnterpriseSettingsModel(QObject* parent = nullptr);
      ~EnterpriseSettingsModel() = default;

      void SetSettingsClient(Settings::SettingsClientInterface* settingsClient);

      int rowCount(const QModelIndex& parent = QModelIndex()) const override;
      int columnCount(const QModelIndex& parent = QModelIndex()) const override;
      QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
      QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
      Qt::ItemFlags flags(const QModelIndex& index) const override;
      bool setData(const QModelIndex& index,
                   const QVariant& value,
                   int role) override;

   signals:
      void SettingsUpdated();
      void SettingsPopulated();

   public slots:
      void HandleSystemSettingChanged(Settings::Setting setting,
                                      const QVariant& value);
      void HandleDisplaySettingChanged(Settings::Setting setting,
                                       unsigned int display,
                                       const QVariant& value);

   private:
      int GetIndexToInsertSetting(Settings::Setting setting) const;
      int GetIndexOfSetting(Settings::Setting setting) const;
      void PopulateSettings();
      bool WriteEditedSetting(Settings::Setting setting, const QVariant& value);

      Settings::SettingsClientInterface* SettingsClient;
      QList<RowData> Data;
      bool DatabaseStarted;
   };
}
