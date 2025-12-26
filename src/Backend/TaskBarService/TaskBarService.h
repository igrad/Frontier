#pragma once

#include "TaskBarTypes.h"
#include "TaskBarServiceInterface.h"

#include <SettingsClient.h>

namespace TaskBar
{
   class TaskBarService: public TaskBarServiceInterface
   {
      Q_OBJECT

   public:
      explicit TaskBarService(QObject* parent = nullptr);
      ~TaskBarService() = default;

   public slots:
      void HandleSettingTaskBarAlignmentChanged(const QVariant& value) override;
      void HandleSettingTaskBarDirectionalityChanged(const QVariant& value) override;

   private:
      void SubscribeToSettings();

      Settings::SettingsClient SettingsClient;
      Alignment CurrentAlignment;
      Directionality CurrentDirectionality;
   };
}
