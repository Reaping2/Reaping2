#ifndef INCLUDED_MAP_PROPERTY_EDITOR_BASE_SYSTEM_H
#define INCLUDED_MAP_PROPERTY_EDITOR_BASE_SYSTEM_H

#include "platform/i_platform.h"
#include "engine/system.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "editor_back_event.h"
#include "../../engine/engine.h"
#include "ui/ui.h"
#include "room_editor_loaded_event.h"
#include "level_generator/i_room.h"

namespace map {

template<typename EDITOR, typename PROPERTY>
class PropertyEditorBaseSystem : public engine::System
{
public: 
    PropertyEditorBaseSystem(std::string const& HUD, std::string const& mode);
protected:
    std::string mHUD;
    std::string mMode;
    void HandleModeChanged( map::EditorModeChangedEvent const& Evt );
    void HandleSpace();
    void HandleEditorBack( map::EditorBackEvent const& Evt );
    virtual void LoadPropNames();
    std::vector<std::string> mPropNames;
    Opt<RoomDesc> mRoomDesc;
    AutoReg mOnRoomEditorLoaded;
    void OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt );
    virtual void Init();
};

template<typename EDITOR, typename PROPERTY>
void map::PropertyEditorBaseSystem<EDITOR, PROPERTY>::LoadPropNames()
{
    mPropNames.clear();
    if (!mRoomDesc.IsValid())
    {
        return;
    }

    for (auto&& prop : mRoomDesc->GetRoom()->GetProperties())
    {
        if (prop.GetType() == PROPERTY::GetType_static())
        {
            mPropNames.push_back( "prop" + std::to_string( prop.GetUID() ) );
        }
    }
}

template<typename EDITOR, typename PROPERTY>
void PropertyEditorBaseSystem<EDITOR, PROPERTY>::OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt )
{
    mRoomDesc = Evt.mRoomDesc;
    LoadPropNames();
}

template<typename EDITOR, typename PROPERTY>
void PropertyEditorBaseSystem<EDITOR, PROPERTY>::Init()
{
    mOnRoomEditorLoaded = EventServer<map::RoomEditorLoadedEvent>::Get().Subscribe( boost::bind( &PropertyEditorBaseSystem::OnRoomEditorLoaded, this, _1 ) );
}


template<typename EDITOR, typename PROPERTY>
void map::PropertyEditorBaseSystem<EDITOR, PROPERTY>::HandleEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (!Evt.mBackToBaseHud)
        {
            Ui::Get().Load( mHUD );
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

template<typename EDITOR, typename PROPERTY>
void PropertyEditorBaseSystem<EDITOR, PROPERTY>::HandleSpace()
{
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    if (keyboard->GetKey( GLFW_KEY_SPACE ).State == KeyState::Typed)
    {
        if (EditorHudState::Get().IsHudShown())
        {
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
        }
        else
        {
            Ui::Get().Load( mHUD );
            EditorHudState::Get().SetHudShown( true );
        }
    }
}

template<typename EDITOR, typename PROPERTY>
PropertyEditorBaseSystem<EDITOR, PROPERTY>::PropertyEditorBaseSystem( std::string const& HUD, std::string const& mode )
    : mHUD(HUD)
    , mMode(mode)
{

}

template<typename EDITOR, typename PROPERTY>
void PropertyEditorBaseSystem<EDITOR, PROPERTY>::HandleModeChanged( map::EditorModeChangedEvent const& Evt )
{
    if (Evt.mMode == mMode)
    {
        ::engine::Engine::Get().SetEnabled<EDITOR>( true );
        Ui::Get().Load( mHUD );
        EditorHudState::Get().SetHudShown( true );
    }
    else
    {
        ::engine::Engine::Get().SetEnabled<EDITOR>( false );
    }
}

} // namespace map

#endif//INCLUDED_MAP_PROPERTY_EDITOR_BASE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "property_editor_base_system" -n "map"
