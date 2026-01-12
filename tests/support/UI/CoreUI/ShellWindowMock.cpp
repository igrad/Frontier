#include "ShellWindowMock.h"

void ShellWindowMock::EmitClosed()
{
   emit Closed();
}
