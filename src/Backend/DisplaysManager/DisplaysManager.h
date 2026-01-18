#pragma once

#include "DisplaysManagerInterface.h"

#include <QMap>

class WindowsAPIInterface;

class DisplaysManager: public DisplaysManagerInterface
{
   Q_OBJECT

public:
   DisplaysManager(WindowsAPIInterface& windowsAPI,
                   QObject* parent = nullptr);
   ~DisplaysManager() = default;

   void RegisterMetaTypes() const override;

   void PollDisplaysInfo() override;

private slots:
   void HandleDisplayDetected(const DisplayInfo& info);
   void HandleNumberOfDisplaysChanged(uint8_t numDisplays);

private:
   void ConnectToWindowsAPI();
   void FinalizeCurrentEvent();

   WindowsAPIInterface& WindowsAPI;
   uint8_t NumDisplays;
   QMap<QString, DisplayInfo> Displays;

   DisplayConfigEvent CurrentEvent;
};
