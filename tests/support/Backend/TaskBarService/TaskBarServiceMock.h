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

      void EmitViewDataChanged(const ViewData& data);
   };
}
