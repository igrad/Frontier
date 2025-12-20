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

const std::string SettingsClient::GetSettingHandlerMethodStr(Setting setting, bool normalized) const
{
   // This weirdness is just so that we can validate the handler function exists before we make the
   // connection. That way we don't have to validate at the time of setting value change.
   std::string str;
   if(normalized)
   {
      const QString rawStr = QString("HandleSetting%1Changed(const QVariant&)")
                                .arg(Settings::ToString(setting));
      str = QMetaObject::normalizedSignature(rawStr.toStdString().c_str()).toStdString();
   }
   else
   {
      const QString rawStr = QString("HandleSetting%1Changed")
                                .arg(Settings::ToString(setting));
      str = rawStr.toStdString();
   }

   return str;
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
   const std::string methodStr = GetSettingHandlerMethodStr(setting, true);
   if(nullptr != subscriber)
   {
      if((0 <= subscriber->metaObject()->indexOfMethod(methodStr.c_str())))
      {
         Subscriptions.insert(setting, subscriber);
      }
      else
      {
         LogError(QString("Could not find method %1 to handle setting %2 in object %3")
                     .arg(methodStr.c_str(),
                          ToString(setting),
                          subscriber->metaObject()->className()));
      }
   }
   else
   {
      LogError("A nullptr cannot subscribe to a setting!");
   }
}

void SettingsClient::HandleSettingUpdated(const Setting& setting, const QVariant& value)
{
   QList<QObject*> subscribers = Subscriptions.values();

   const std::string methodStr = GetSettingHandlerMethodStr(setting);

   for(QObject* sub : std::as_const(subscribers))
   {
      QMetaObject::invokeMethod(sub,
                                methodStr.c_str(),
                                value);
   }
}
