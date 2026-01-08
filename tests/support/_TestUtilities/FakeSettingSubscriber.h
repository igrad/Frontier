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
   void _TestSettingReceived(const QVariant& value);

public slots:
   void HandleSetting_TestSettingChanged(const QVariant& value)
   {
      emit _TestSettingReceived(value);
   }
};

class FakeAllSettingsSubscriber: public FakeSettingSubscriber
{
   Q_OBJECT

public:
   FakeAllSettingsSubscriber() = default;
   ~FakeAllSettingsSubscriber() = default;

signals:
   void SettingChangeReceived(const Settings::Setting setting, const QVariant& value);

public slots:
   void HandleSettingChanged(const Settings::Setting setting, const QVariant& value)
   {
      emit SettingChangeReceived(setting, value);
   }
};
