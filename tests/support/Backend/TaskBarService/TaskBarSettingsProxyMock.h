#pragma once

#include <TaskBarSettingsProxyInterface.h>

#include <gmock/gmock.h>

namespace TaskBar
{
   class TaskBarSettingsProxyMock: public TaskBarSettingsProxyInterface
   {
      Q_OBJECT

   public:
      TaskBarSettingsProxyMock() = default;
      ~TaskBarSettingsProxyMock() = default;

      void EmitSettingsChanged(const QString& displayID);

      MOCK_METHOD(Alignment,
                  GetAlignment,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(bool,
                  GetAutoHide,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(int,
                  GetAutoHideDelayMs,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(int,
                  GetOpacity,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(Orientation,
                  GetOrientation,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(QRect,
                  GetRect,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(bool,
                  GetShown,
                  (const QString& displayID),
                  (const override));
      MOCK_METHOD(bool,
                  GetStartButtonShown,
                  (const QString& displayID),
                  (const override));

   // slots:
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarAlignmentChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarAutoHideChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarAutoHideDelayMsChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarOpacityChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarOrientationChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarRectChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarShownChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
      MOCK_METHOD(void,
                  HandleDisplaySettingTaskBarStartButtonShownChanged,
                  (const QString& displayID, const QVariant& value),
                  (override));
   };
}
