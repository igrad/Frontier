#include "SettingsClient.h"
#include "SettingsService.h"

#include <Log.h>

using namespace Settings;

SettingsService* SettingsClient::SettingsService = nullptr;

SettingsClient::SettingsClient(const QString& owner)
   : Owner(owner)
{
   ConnectToService();
}

SettingsClient::~SettingsClient()
{

}

void SettingsClient::ConnectToService()
{
   if(nullptr != SettingsClient::SettingsService)
   {
      // Prevents a clang warning about bitwise OR (|) op on these connection types
      // NOLINTNEXTLINE
      auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::QueuedConnection);
      connect(SettingsClient::SettingsService, &SettingsService::SettingUpdated,
              this, &SettingsClient::HandleSettingUpdated,
              conn);
      connect(this, &SettingsClient::WriteSettingValue,
              SettingsClient::SettingsService, &SettingsService::HandleWriteSettingValue,
              conn);
   }
   else
   {
      LogError("SettingsClient::SettingsService is null at time of client instantiation");
   }
}

void SettingsClient::SubscribeToSetting(const Setting& setting,
    void(*slot)(const Setting& setting, const QVariant& value))
{
   if(!Subscriptions.contains(setting))
   {
      Subscriptions[setting] = QList<SettingUpdateSlot>{slot};
   }
   else
   {
      Subscriptions[setting].append(slot);
   }
}

void SettingsClient::HandleSettingUpdated(const Setting& setting, const QVariant& value)
{
   auto iter = Subscriptions.find(setting);

   if(Subscriptions.end() != iter)
   {
      QList<SettingUpdateSlot> subscriptionSlots = iter.value();
      for(auto subscriptionSlot : subscriptionSlots)
      {
         subscriptionSlot(setting, value);
      }
   }
}
