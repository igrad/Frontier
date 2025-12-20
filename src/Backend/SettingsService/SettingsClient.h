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

      void SubscribeToSetting(const Setting& setting, QObject* subscriber) override;

   signals:
      void WriteSettingValue(const Setting& setting, const QVariant& value);

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
