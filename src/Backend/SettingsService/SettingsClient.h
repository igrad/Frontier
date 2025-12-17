#pragma once

#include "SettingsClientInterface.h"

#include <QObject>
#include <QMultiHash>

namespace Settings
{
   class SettingsService;

   class SettingsClient: public SettingsClientInterface
   {
      Q_OBJECT

   public:
      explicit SettingsClient(const QString& owner);
      ~SettingsClient();

      void SubscribeToSetting(const Setting& setting, QObject* subscriber) override;

   signals:
      void WriteSettingValue(const Setting& setting, const QVariant& value);

   private slots:
      void HandleSettingUpdated(const Setting& setting, const QVariant& value);

   private:
      friend class SettingsService;
      static SettingsService* SettingsService;

      void ConnectToService();

      QString Owner;
      QMultiHash<Setting, QObject*> Subscriptions;
   };
}
