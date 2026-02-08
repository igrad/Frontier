#include <TaskBar/TaskBarView.h>

#include <TaskBarService/TaskBarServiceMock.h>
#include <ShellWindowMock.h>

#include <TestMacros.h>

#include <gtest/gtest.h>

using namespace TaskBar;
using namespace testing;

class TaskBarViewTest: public Test
{
public:
   TaskBarViewTest()
      : ShellWindow()
      , Service()
      , Info()
      , View(nullptr)
   {

   }

   void SetUpView()
   {
      View.reset(new TaskBarView(XPtr<TaskBarServiceInterface>(&Service),
                                 &ShellWindow,
                                 Info));
   }

   NiceMock<ShellWindowMock> ShellWindow;
   NiceMock<TaskBarServiceMock> Service;
   DisplayInfo Info;
   std::unique_ptr<TaskBarView> View;
};

TEST_F(TaskBarViewTest, SetDimensions1)
{
   GWT("TaskBarView already exists",
       "ViewData changes with valid task bar rect dimensions",
       "The task bar rect updates to match the new dimensions");

   const QString displayID = "DisplayID";
   Info.ID = displayID;
   Info.Rect = {0, 0, 1920, 1080};
   SetUpView();

   ViewData data;
   data.Alignment = Alignment::Bottom;
   data.Direction = Direction::LeftToRight;
   data.Rect = {0, 0, 1920, 100};
   data.DisplayID = displayID;

   View->HandleViewDataChanged(displayID, data);

   EXPECT_EQ(data.Rect, View->geometry());
}
