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
      , Info()
      , View(XPtr<TaskBarServiceInterface>(&Service), &ShellWindow, Info)
   {

   }

   NiceMock<ShellWindowMock> ShellWindow;
   NiceMock<TaskBarServiceMock> Service;
   DisplayInfo Info;
   TaskBarView View;
};
