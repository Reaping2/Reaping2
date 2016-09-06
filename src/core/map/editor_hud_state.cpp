#include "editor_hud_state.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace map {


bool EditorHudState::IsHudShown() const
{
    return mHudShown;
}

void EditorHudState::SetHudShown( bool shown )
{
    mHudShown = shown;
}

} // namespace map

