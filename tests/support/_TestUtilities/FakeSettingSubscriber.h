#pragma once

#include <SettingsService/Settings.h>

#include <QObject>

class FakeSettingSubscriber: public QObject
{
   Q_OBJECT

public:
   FakeSettingSubscriber() = default;
   ~FakeSettingSubscriber() = default;

signals:
   void System_TestSettingReceived(const QVariant& value);
   void Display_TestSettingReceived(const QString& displayID,
                                    const QVariant& value);

public slots:
   void HandleSystemSetting_TestSettingChanged(const QVariant& value)
   {
      emit System_TestSettingReceived(value);
   }

   void HandleDisplaySetting_TestSettingChanged(const QString& displayID,
                                                const QVariant& value)
   {
      emit Display_TestSettingReceived(displayID, value);
   }
};

class FakeAllSettingsSubscriber: public FakeSettingSubscriber
{
   Q_OBJECT

public:
   FakeAllSettingsSubscriber() = default;
   ~FakeAllSettingsSubscriber() = default;

signals:
   void SystemSettingChangeReceived(const Settings::Setting setting,
                                    const QVariant& value);
   void DisplaySettingChangeReceived(const Settings::Setting setting,
                                     const QString& displayID,
                                     const QVariant& value);

public slots:
   void HandleSystemSettingChanged(const Settings::Setting setting, const QVariant& value)
   {
      emit SystemSettingChangeReceived(setting, value);
   }

   void HandleDisplaySettingChanged(const Settings::Setting setting,
                                    const QString& displayID,
                                    const QVariant& value)
   {
      emit DisplaySettingChangeReceived(setting, displayID, value);
   }
};
