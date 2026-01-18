#pragma once

#include "DisplayInfo.h"

#include <QObject>

class DisplaysManagerInterface: public QObject
{
   Q_OBJECT

public:
   ~DisplaysManagerInterface() = default;

   virtual void RegisterMetaTypes() const = 0;
   virtual void PollDisplaysInfo() = 0;

signals:
   void DisplayConfigChanged(const DisplayEvent& event,
                             const QSet<DisplayInfo>& displays);
};
