#ifndef INCLUDED_MAP_EDITOR_HUD_STATE_H
#define INCLUDED_MAP_EDITOR_HUD_STATE_H

#include "platform/singleton.h"

namespace map {

class EditorHudState : public platform::Singleton<EditorHudState>
{
protected:
    friend class platform::Singleton<EditorHudState>;
    EditorHudState() = default;
    bool mHudShown = false;
public:
    bool IsHudShown() const;
    void SetHudShown( bool shown );
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_HUD_STATE_H


//command:  "classgenerator.exe" -g "enum" -n "map" -c "editor_hud_state" -m "bool-hudShown"
