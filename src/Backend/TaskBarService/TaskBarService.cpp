#include "TaskBarService.h"

using namespace TaskBar;

TaskBarService::TaskBarService(QObject* parent)
   : SettingsProxy(this)
   , Workers()
{
   setParent(parent);
}

void TaskBarService::RegisterMetaTypes() const
{
   qRegisterMetaType<TaskBar::ViewData>("TaskBar::ViewData");
}

void TaskBarService::HandleDisplayConfigChanged(const DisplayEvent& event,
                                                const QSet<DisplayInfo>& displays)
{

}

void TaskBarService::HandleSettingsChanged(uint8_t displayNum)
{
   Workers[displayNum]->HandleSettingsChanged();
}
