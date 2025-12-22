#pragma once

#include <QWidget>

// What features should our overlays have? Whatever the Windows Settings menu can do
// min, max, X buttons
// Right-click menu bar to open a context menu (can be disabled)
//		Restore, Move, Minimize, Maximize | Close
// Resize by dragging the corners or edges
// Assign to an owning executable
// Opening the
namespace UI
{
   class Overlay : public QWidget
   {
      Q_OBJECT

   public:
      explicit Overlay(const QString& name);
      ~Overlay() = default;

   private:
      const QString Name;
   };
}
