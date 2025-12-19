#include "SettingsClient.h"
#include "SettingsServiceInterface.h"

#include <Log.h>

using namespace Settings;

SettingsServiceInterface* SettingsClient::Service = nullptr;

SettingsClient::SettingsClient(const QString& owner)
   : Owner(owner)
{
   ConnectToService();
}

SettingsClient::~SettingsClient()
{

}

// NOTE: For testing purposes only.
const SettingsServiceInterface* SettingsClient::GetSettingsServicePtr()
{
   return SettingsClient::Service;
}

void SettingsClient::ConnectToService()
{
   if(nullptr != SettingsClient::Service)
   {
      // Prevents a clang warning about bitwise OR (|) op on these connection types
      // NOLINTNEXTLINE
      auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection | Qt::QueuedConnection);
      connect(SettingsClient::Service, &SettingsServiceInterface::SettingUpdated,
              this, &SettingsClient::HandleSettingUpdated,
              conn);
      connect(this, &SettingsClient::WriteSettingValue,
              SettingsClient::Service, &SettingsServiceInterface::HandleWriteSettingValue,
              conn);
   }
   else
   {
      LogError("SettingsClient::SettingsService is null at time of client instantiation");
   }
}

void SettingsClient::SubscribeToSetting(const Setting& setting, QObject* subscriber)
{
   Subscriptions.insert(setting, subscriber);
}

void SettingsClient::HandleSettingUpdated(const Setting& setting, const QVariant& value)
{
   QList<QObject*> subscribers = Subscriptions.values();

   const std::string methodQStr = QString("HandleSetting%1Changed")
                                     .arg(Settings::ToString(setting))
                                     .toStdString();
   const char* methodStr = methodQStr.c_str();

   for(QObject* sub : std::as_const(subscribers))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   methodStr,
                                   Q_ARG(QVariant, value),
                                   Qt::QueuedConnection);
      }
   }
}
