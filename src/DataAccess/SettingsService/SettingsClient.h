#pragma once

#include "SettingsClientInterface.h"

#include <QObject>
#include <QMultiHash>

class SettingsServicePointerHelper;

namespace Settings
{
   class SettingsServiceInterface;

   class SettingsClient: public SettingsClientInterface
   {
      Q_OBJECT

   public:
      SettingsClient(const QString& owner,
                     QObject* parent = nullptr);
      ~SettingsClient();

      static const SettingsServiceInterface* GetSettingsServicePtr();

      bool SubscribeToSystemSetting(Setting setting, QObject* subscriber) override;
      bool SubscribeToDisplaySetting(Setting setting, QObject* subscriber) override;
      bool SubscribeToAllSystemSettings(QObject* subscriber) override;
      bool SubscribeToAllDisplaySettings(QObject* subscriber) override;
      bool WriteSystemSettingValue(Setting setting, const QVariant& value) override;
      bool WriteDisplaySettingValue(Setting setting,
                                    uint8_t display,
                                    const QVariant& value) override;

   private slots:
      void HandleSystemSettingUpdated(const Setting& setting, const QVariant& value);
      void HandleDisplaySettingUpdated(const Setting& setting,
                                       uint8_t display,
                                       const QVariant& value);

   private:
      friend class SettingsService;
      friend class ::SettingsServicePointerHelper;
      static SettingsServiceInterface* Service;

      bool SubscribeToSetting(const std::string& methodStr,
                              Setting setting,
                              QObject* subscriber);
      const std::string GetSystemSettingHandlerMethodStr(Setting setting,
                                                         bool normalized = false) const;
      const std::string GetDisplaySettingHandlerMethodStr(Setting setting,
                                                         bool normalized = false) const;
      void ConnectToService();

      QString Owner;
      QMultiHash<Setting, QObject*> Subscriptions;
      QList<QObject*> ObjectsSubscribedToAllSystemSignals;
      QList<QObject*> ObjectsSubscribedToAllDisplaySignals;
   };
}
