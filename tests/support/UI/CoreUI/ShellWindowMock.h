#pragma once

#include <ShellWindowInterface.h>

#include <gmock/gmock.h>

class ShellWindowMock: public ShellWindowInterface
{
   Q_OBJECT

public:
   void EmitClosed();

   MOCK_METHOD(void, HandleDisplayConfigChanged, (const DisplayInfo&), (override));
};
