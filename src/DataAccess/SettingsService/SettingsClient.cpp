#include "SettingsClient.h"
#include "SettingsServiceInterface.h"

#include <Log.h>
#include <Utilities/MethodLookupHelpers.h>

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

bool SettingsClient::SubscribeToSystemSetting(Setting setting, QObject* subscriber)
{
   const std::string methodStr = GetSystemSettingHandlerMethodStr(setting, true);
   return SubscribeToSetting(methodStr, setting, subscriber);
}

bool SettingsClient::SubscribeToDisplaySetting(Setting setting, QObject* subscriber)
{
   const std::string methodStr = GetDisplaySettingHandlerMethodStr(setting, true);
   return SubscribeToSetting(methodStr, setting, subscriber);
}

bool SettingsClient::SubscribeToAllSystemSettings(QObject* subscriber)
{
   bool retVal = false;

   const std::string methodStr = GetSystemSettingHandlerMethodStr(Setting::_All, true);
   if(nullptr != subscriber)
   {
      if(0 <= subscriber->metaObject()->indexOfMethod(methodStr.c_str()))
      {
         ObjectsSubscribedToAllSystemSignals.push_back(subscriber);
         retVal = true;
      }
      else
      {
         LogError(QString("Could not find method %1 to handle all settings in object %3")
                     .arg(methodStr.c_str(),
                          subscriber->metaObject()->className()));
      }
   }

   return retVal;
}

bool SettingsClient::SubscribeToAllDisplaySettings(QObject* subscriber)
{
   bool retVal = false;

   const std::string methodStr = GetDisplaySettingHandlerMethodStr(Setting::_All, true);
   if(nullptr != subscriber)
   {
      if(0 <= subscriber->metaObject()->indexOfMethod(methodStr.c_str()))
      {
         ObjectsSubscribedToAllDisplaySignals.push_back(subscriber);
         retVal = true;
      }
      else
      {
         LogError(QString("Could not find method %1 to handle all settings in object %3")
                     .arg(methodStr.c_str(),
                          subscriber->metaObject()->className()));
      }
   }

   return retVal;
}

bool SettingsClient::WriteSystemSettingValue(Setting setting, const QVariant& value)
{
   bool retVal = false;

   if(Setting::None != setting)
   {
      emit CacheSystemSettingValue(setting, value);
      retVal = true;
   }
   else
   {
      LogError("Cannot write to None setting. This is a code error.");
   }

   return retVal;
}

bool SettingsClient::WriteDisplaySettingValue(Setting setting,
                                              const QString& displayID,
                                              const QVariant& value)
{
   bool retVal = false;

   if(Setting::None != setting)
   {
      emit CacheDisplaySettingValue(setting, displayID, value);
      retVal = true;
   }
   else
   {
      LogError("Cannot write to None setting. This is a code error.");
   }

   return retVal;
}

void SettingsClient::HandleSystemSettingUpdated(const Setting& setting, const QVariant& value)
{
   const QList<QObject*> subscribers = Subscriptions.values(setting);

   const std::string methodStr = GetSystemSettingHandlerMethodStr(setting);

   for(QObject* sub : std::as_const(subscribers))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   methodStr.c_str(),
                                   value);
      }
   }

   constexpr const char* allSubStr = "HandleSystemSettingChanged";
   for(QObject* sub : std::as_const(ObjectsSubscribedToAllSystemSignals))
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

void SettingsClient::HandleDisplaySettingUpdated(const Setting& setting,
                                                 const QString& displayID,
                                                 const QVariant& value)
{
   const QList<QObject*> subscribers = Subscriptions.values(setting);

   const std::string methodStr = GetDisplaySettingHandlerMethodStr(setting);

   for(QObject* sub : std::as_const(subscribers))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   methodStr.c_str(),
                                   value);
      }
   }

   constexpr const char* allSubStr = "HandleDisplaySettingChanged";
   for(QObject* sub : std::as_const(ObjectsSubscribedToAllDisplaySignals))
   {
      if(nullptr != sub)
      {
         QMetaObject::invokeMethod(sub,
                                   allSubStr,
                                   Q_ARG(Setting, setting),
                                   Q_ARG(QString, displayID),
                                   Q_ARG(QVariant, value));
      }
   }
}

bool SettingsClient::SubscribeToSetting(const std::string& methodStr,
                                        Setting setting,
                                        QObject* subscriber)
{
   bool retVal = false;
   if(nullptr != subscriber)
   {
      if(QObjectHasMethodDeclared(subscriber, methodStr))
      {
         Subscriptions.insert(setting, subscriber);
         retVal = true;
      }
      else
      {
         LogError(QString("Could not find slot \"%1\" to handle setting %2 in object of class %3")
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

const std::string SettingsClient::GetSystemSettingHandlerMethodStr(Setting setting,
                                                                   bool normalized) const
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
         rawStr = QString("HandleSystemSettingChanged(const Settings::Setting, const QVariant&)");
      }
      else
      {
         rawStr = QString("HandleSystemSetting%1Changed(const QVariant&)")
                     .arg(ToString(setting));
      }
      str = QMetaObject::normalizedSignature(rawStr.toStdString().c_str()).toStdString();
   }
   else
   {
      QString rawStr;
      if(allSetting)
      {
         rawStr = QString("HandleSystemSettingChanged");
      }
      else
      {
         rawStr = QString("HandleSystemSetting%1Changed")
                     .arg(ToString(setting));
      }
      str = rawStr.toStdString();
   }

   return str;
}

const std::string SettingsClient::GetDisplaySettingHandlerMethodStr(Setting setting,
                                                                    bool normalized) const
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
         rawStr = QString("HandleDisplaySettingChanged(const Settings::Setting, "
                          "uint8_t, "
                          "const QVariant&)");
      }
      else
      {
         rawStr = QString("HandleDisplaySetting%1Changed(const QVariant&, uint8_t)")
                     .arg(ToString(setting));
      }
      str = QMetaObject::normalizedSignature(rawStr.toStdString().c_str()).toStdString();
   }
   else
   {
      QString rawStr;
      if(allSetting)
      {
         rawStr = QString("HandleDisplaySettingChanged");
      }
      else
      {
         rawStr = QString("HandleDisplaySetting%1Changed")
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
      connect(SettingsClient::Service, &SettingsServiceInterface::SystemSettingUpdated,
              this, &SettingsClient::HandleSystemSettingUpdated,
              conn);
      connect(this, &SettingsClient::CacheSystemSettingValue,
              SettingsClient::Service, &SettingsServiceInterface::HandleCacheSystemSettingValue,
              conn);

      connect(SettingsClient::Service, &SettingsServiceInterface::DisplaySettingUpdated,
              this, &SettingsClient::HandleDisplaySettingUpdated,
              conn);
      connect(this, &SettingsClient::CacheDisplaySettingValue,
              SettingsClient::Service, &SettingsServiceInterface::HandleCacheDisplaySettingValue,
              conn);
   }
   else
   {
      LogError(QString("SettingsClient::SettingsService for client \"%1\" is null at "
                       "time of client instantiation. This client will now be orphaned.")
                  .arg(Owner));
   }
}
