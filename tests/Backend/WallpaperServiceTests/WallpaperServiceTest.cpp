#include <WallpaperService.h>

#include <FakeDbHelper.h>
#include <TestMacros.h>

#include <gtest/gtest.h>

using namespace testing;
using namespace Wallpaper;

class WallpaperServiceTest: public Test
{
public:
   WallpaperServiceTest()
      : Db()
      , Service(nullptr)
   {

   }

   void SetUpService()
   {
      Service.reset(new WallpaperService());
   }

   FakeDbHelper Db;
   std::unique_ptr<WallpaperService> Service;
};
