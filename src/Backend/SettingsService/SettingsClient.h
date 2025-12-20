#pragma once

#include "SettingsClientInterface.h"

#include <QObject>
#include <QMultiHash>

class ServicePointerHelper;

namespace Settings
{
   class SettingsServiceInterface;

   class SettingsClient: public SettingsClientInterface
   {
      Q_OBJECT

   public:
      explicit SettingsClient(const QString& owner);
      ~SettingsClient();

      static const SettingsServiceInterface* GetSettingsServicePtr();

      bool SubscribeToSetting(Setting setting, QObject* subscriber) override;
      bool WriteSettingValue(Setting setting, const QVariant& value) override;

   private slots:
      void HandleSettingUpdated(const Setting& setting, const QVariant& value);

   private:
      friend class SettingsService;
      friend class ::ServicePointerHelper;
      static SettingsServiceInterface* Service;

      const std::string GetSettingHandlerMethodStr(Setting setting, bool normalized = false) const;
      void ConnectToService();

      QString Owner;
      QMultiHash<Setting, QObject*> Subscriptions;
   };
}
