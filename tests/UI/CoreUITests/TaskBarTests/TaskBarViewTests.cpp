#include <TaskBar/TaskBarView.h>

#include <TaskBarService/TaskBarServiceMock.h>
#include <ShellWindowMock.h>

#include <TestMacros.h>

#include <gtest/gtest.h>

using namespace TaskBar;
using namespace testing;

class TaskBarViewTest: public Test
{
   TaskBarViewTest()
      : ShellWindow()
      , Service()
      , DisplayNum(0)
      , View(XPtr<TaskBarServiceInterface>(&Service), &ShellWindow, DisplayNum)
   {

   }

   NiceMock<ShellWindowMock> ShellWindow;
   NiceMock<TaskBarServiceMock> Service;
   uint8_t DisplayNum;
   TaskBarView View;
};
