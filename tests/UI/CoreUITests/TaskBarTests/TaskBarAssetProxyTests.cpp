#include <TaskBar/TaskBarAssetProxy.h>

#include <AssetManager/AssetClientMock.h>
#include <TestMacros.h>

using namespace Assets;
using namespace TaskBar;
using namespace testing;

class TaskBarAssetProxyTests: public Test
{
public:
   TaskBarAssetProxyTests()
      : AssetClient()
      , Proxy(AssetClient)
   {
   }

   NiceMock<AssetClientMock> AssetClient;
   TaskBarAssetProxy Proxy;
};
