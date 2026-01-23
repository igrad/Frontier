#pragma once

#include <QMetaType>
#include <QRect>
#include <QMap>

#include <windef.h>

typedef QString DisplayID;

struct DisplayInfo
{
   DisplayID ID = "";			// The EDID of the display hardware
   QString SessionName = "";	// The name assigned to this monitor for this session
   QString DisplayName = "";	// The name of the monitor itself
   HMONITOR Handle;				// The handle used to fetch display info in windows API
   unsigned int Number = 0;	// Number currently assigned to this display
   bool IsPrimary = false;
   QRect Rect = {0, 0, 0, 0};
   unsigned int XDPI = 1;			// 1 just to prevent divide by 0 errors
   unsigned int YDPI = 1;

   bool operator==(const DisplayInfo& rhs) const
   {
      return ID == rhs.ID &&
             SessionName == rhs.SessionName &&
             DisplayName == rhs.DisplayName &&
             Handle == rhs.Handle &&
             Number == rhs.Number &&
             IsPrimary == rhs.IsPrimary &&
             Rect == rhs.Rect &&
             XDPI == rhs.XDPI &&
             YDPI == rhs.YDPI;
   }

   bool operator!=(const DisplayInfo& rhs) const
   {
      return !(*this == rhs);
   }
};

Q_DECLARE_METATYPE(DisplayInfo);

struct DisplayConfigEvent
{
   enum class EventType: int
   {
      None = 0,
      Changed,
      Added,
      Removed
   };

   QMap<DisplayID, QPair<EventType, DisplayInfo>> Displays;
};
typedef DisplayConfigEvent::EventType DisplayConfigEventType;

Q_DECLARE_METATYPE(DisplayConfigEvent);
