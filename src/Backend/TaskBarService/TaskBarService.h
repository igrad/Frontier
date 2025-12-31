#pragma once

#include "TaskBarTypes.h"
#include "TaskBarServiceInterface.h"

#include <SettingsService/SettingsClient.h>

namespace TaskBar
{
   class TaskBarService: public TaskBarServiceInterface
   {
      Q_OBJECT

   public:
      explicit TaskBarService(QObject* parent = nullptr);
      ~TaskBarService() = default;

      void RegisterMetaTypes() const override;

   public slots:
      void HandleSettingTaskBarAlignmentChanged(const QVariant& value) override;
      void HandleSettingTaskBarOrientationChanged(const QVariant& value) override;

   private:
      void SubscribeToSettings();

      Settings::SettingsClient SettingsClient;
      Alignment CurrentAlignment;
      Orientation CurrentOrientation;
   };
}
