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
   void DisplayDetected(const DisplayInfo& info);
   void NumberOfDisplaysChanged(uint8_t numDisplays);
   void ENTERPRISE_DisplayInfoModified(const DisplayConfigEvent& event);

public slots:
   virtual void HandlePollDisplaysInfo() = 0;
};
