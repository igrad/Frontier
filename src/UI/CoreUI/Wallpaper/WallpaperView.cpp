#include "WallpaperView.h"

#include <WallpaperService.h>

using namespace Wallpaper;

WallpaperView::WallpaperView(WallpaperService* service)
   : CurrentData()
{
   ConnectToServiceSignals(service);
}

WallpaperView::~WallpaperView()
{

}

void WallpaperView::HandleWallpaperDataChanged(const WallpaperData& data)
{
   CurrentData = data;

   // Run a switch on the WallpaperImageType, and invoke specific handler functions appropriately
}

void WallpaperView::ConnectToServiceSignals(WallpaperService* service)
{
   if(nullptr != service)
   {
      // Prevents a clang warning about bitwise OR (|) op on these connection types
      // NOLINTNEXTLINE
      const auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection |
                                                        Qt::QueuedConnection);

      connect(service, &WallpaperService::WallpaperDataChanged,
              this, &WallpaperView::HandleWallpaperDataChanged,
              conn);
   }
}
