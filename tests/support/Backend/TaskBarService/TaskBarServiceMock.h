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
                  (const DisplayConfigEvent&),
                  (override));

      void EmitViewDataChanged(const DisplayID& displayID, const ViewData& data);
   };
}
