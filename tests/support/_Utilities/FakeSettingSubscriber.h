#pragma once

#include <QObject>

class FakeSettingSubscriber: public QObject
{
   Q_OBJECT

public:
   FakeSettingSubscriber() = default;
   ~FakeSettingSubscriber() = default;

signals:
   void TestSettingReceived(const QVariant& value);

public slots:
   void HandleSettingTestSettingChanged(const QVariant& value)
   {
      emit TestSettingReceived(value);
   }
};
