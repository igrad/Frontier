#include "SettingsClient.h"
#include "SettingsServiceInterface.h"

#include <Log.h>

using namespace Settings;

SettingsServiceInterface* SettingsClient::Service = nullptr;

SettingsClient::SettingsClient(const QString& owner, QObject* parent)
   : Owner(owner)
{
   setParent(parent);
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
   const bool allSetting = Setting::_All == setting;
   if(normalized)
   {
      QString rawStr;
      if(allSetting)
      {
         rawStr = QString("HandleSettingChanged(const Settings::Setting, const QVariant&)");
      }
      else
      {
         rawStr = QString("HandleSetting%1Changed(const QVariant&)")
                     .arg(ToString(setting));
      }
      str = QMetaObject::normalizedSignature(rawStr.toStdString().c_str()).toStdString();
   }
   else
   {
      QString rawStr;
      if(allSetting)
      {
         rawStr = QString("HAndleSettingChanged");
      }
      else
      {
         rawStr = QString("HandleSetting%1Changed")
                     .arg(ToString(setting));
      }
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
      const auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection |
                                                        Qt::QueuedConnection);
      connect(SettingsClient::Service, &SettingsServiceInterface::SettingUpdated,
              this, &SettingsClient::HandleSettingUpdated,
              conn);
      connect(this, &SettingsClient::CacheSettingValue,
              SettingsClient::Service, &SettingsServiceInterface::HandleCacheSettingValue,
              conn);
   }
   else
   {
      LogError(QString("SettingsClient::SettingsService for client \"%1\" is null at "
                       "time of client instantiation")
                  .arg(Owner));
   }
}

bool SettingsClient::SubscribeToSetting(Setting setting, QObject* subscriber)
{
   bool retVal = false;
   const std::string methodStr = GetSettingHandlerMethodStr(setting, true);
   if(nullptr != subscriber)
   {
      if((0 <= subscriber->metaObject()->indexOfMethod(methodStr.c_str())))
      {
         Subscriptions.insert(setting, subscriber);
         retVal = true;
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

   return retVal;
}

bool SettingsClient::SubscribeToAllSettings(QObject* subscriber)
{
   bool retVal = false;

   const std::string methodStr = GetSettingHandlerMethodStr(Setting::_All, true);
   if(nullptr != subscriber)
   {
      if(0 <= subscriber->metaObject()->indexOfMethod(methodStr.c_str()))
      {
         Subscriptions.insert(Setting::_All, subscriber);
         retVal = true;
      }
      else
      {
         LogError(QString("Could not find method %1 to handle setting %2 in object %3")
                     .arg(methodStr.c_str(),
                          ToString(Setting::_All),
                          subscriber->metaObject()->className()));
      }
   }

   return retVal;
}

bool SettingsClient::WriteSettingValue(Setting setting, const QVariant& value)
{
   bool retVal = false;

   if(Setting::None != setting)
   {
      emit CacheSettingValue(setting, value);
      retVal = true;
   }
   else
   {
      LogError("Cannot write to None setting. This is a code error.");
   }

   return retVal;
}

void SettingsClient::HandleSettingUpdated(const Setting& setting, const QVariant& value)
{
   const QList<QObject*> subscribers = Subscriptions.values(setting);

   const std::string methodStr = GetSettingHandlerMethodStr(setting);

   for(QObject* sub : std::as_const(subscribers))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   methodStr.c_str(),
                                   value);
      }
   }

   constexpr const char* allSubStr = "HandleSettingChanged";
   for(QObject* sub : std::as_const(ObjectsSubscribedToAllSignals))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   allSubStr,
                                   Q_ARG(Setting, setting),
                                   Q_ARG(QVariant, value));
      }
   }
}
