#pragma once

#include "WindowsAPISettings.h"

#include <DisplayInfo.h>

class WindowsAPIInterface: public QObject
{
   Q_OBJECT

public:
   ~WindowsAPIInterface() = default;

   virtual QVariant GetCurrentSettingValue(Windows::Setting setting) = 0;

signals:
   void DisplaysDetected(QList<DisplayInfo> info);

public slots:
   virtual void HandlePollDisplaysInfo() = 0;
};
