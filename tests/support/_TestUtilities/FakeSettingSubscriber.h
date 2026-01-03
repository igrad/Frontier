#pragma once

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
