#pragma once

#include <ShellWindowInterface.h>

class ShellWindowMock: public ShellWindowInterface
{
   Q_OBJECT

public:
   void EmitClosed();
};
