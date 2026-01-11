#pragma once

#include "SettingsServiceMock.h"
#include <SettingsClient.h>

struct SettingsServicePointerHelper
{
   SettingsServicePointerHelper(::testing::NiceMock<SettingsServiceMock>* service)
   {
      Settings::SettingsClient::Service = service;
   }
};
