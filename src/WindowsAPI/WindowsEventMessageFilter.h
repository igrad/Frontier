#pragma once

#include "WindowsAPISettings.h"

#include <QAbstractNativeEventFilter>
#include <QObject>

struct tagMSG;
typedef tagMSG MSG;

class WindowsEventMessageFilter: public QObject, public QAbstractNativeEventFilter
{
   Q_OBJECT

public:
   explicit WindowsEventMessageFilter(QObject* parent = nullptr);
   ~WindowsEventMessageFilter() = default;

   bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr*) override;

signals:
   void SettingUpdated(const Windows::Setting setting, const QVariant& value);
   void DevicesChanged();

private:
   bool HandleGenericMessage(void* message);
   bool HandleDeviceChange(const MSG* msg);
};
