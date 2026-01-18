#pragma once

#include <TaskBarServiceInterface.h>

#include <gmock/gmock.h>

namespace TaskBar
{
   class TaskBarServiceMock: public TaskBarServiceInterface
   {
      Q_OBJECT

   public:
      MOCK_METHOD(void, RegisterMetaTypes, (), (const, override));
      MOCK_METHOD(void,
                  HandleDisplayConfigChanged,
                  (const DisplayEvent&, (const QMap<QString, DisplayInfo>&)),
                  (override));

      void EmitViewDataChanged(uint8_t display, const ViewData& data);
   };
}
