#include "platform/i_platform.h"
#include "room_plain_property_editor_system.h"
#include "ui/ui.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "../../engine/engine.h"
#include "../../input/keyboard.h"
#include "editor_hud_state.h"
#include "input/mouse.h"
#include "layer_selected_event.h"
#include "map_system.h"
#include "../renderable_layer.h"
#include "../../platform/id_storage.h"

namespace map {

RoomPlainPropertyEditorSystem::RoomPlainPropertyEditorSystem()
    : mScene( Scene::Get() )
    , mEditorPropModel( "editor_plain_prop", &RootModel::Get() )
    , mPropNamesModel( (ModelValue::get_string_vec_t) RefTo( mPropNames ), "props", &mEditorPropModel )
    , mPropSelectedModel( StringFunc( this, &RoomPlainPropertyEditorSystem::OnPropSelected ), "prop_selected", &mEditorPropModel )
    , mPropStatesModel( (ModelValue::get_string_vec_t) RefTo( mPropStates ), "prop_states", &mEditorPropModel )
{
}


void RoomPlainPropertyEditorSystem::Init()
{
    mOnEditorModeChanged = EventServer<map::EditorModeChangedEvent>::Get().Subscribe( boost::bind( &RoomPlainPropertyEditorSystem::OnEditorModeChanged, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomPlainPropertyEditorSystem::OnEditorBack, this, _1 ) );
    mOnRoomEditorLoaded = EventServer<map::RoomEditorLoadedEvent>::Get().Subscribe( boost::bind( &RoomPlainPropertyEditorSystem::OnRoomEditorLoaded, this, _1 ) );

    auto& idStorage = IdStorage::Get();
    for (auto idToProp : RoomProperty::Get().GetIdToRoomPropertyMap().left)
    {
        std::string name;
        if (idStorage.GetName( idToProp.first, name ))
        {
            mPropNames.push_back( name );
        }
    }
}


void RoomPlainPropertyEditorSystem::Update(double DeltaTime)
{
    if (!mEnabled)
    {
        return;
    }
    static Opt<engine::KeyboardSystem> keyboard = ::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    static Opt<engine::MouseSystem> mouse = ::engine::Engine::Get().GetSystem<MouseSystem>();
}

void RoomPlainPropertyEditorSystem::OnEditorModeChanged(map::EditorModeChangedEvent const& Evt)
{
    if (Evt.mMode == "plain_prop")
    {
        Ui::Get().Load( "editor/all_plain_props" );
    }
    else
    {
    }
}

void RoomPlainPropertyEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
}

void RoomPlainPropertyEditorSystem::EnableSubsystems( bool enable )
{
}

Opt<RoomPlainPropertyEditorSystem> RoomPlainPropertyEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<RoomPlainPropertyEditorSystem>();
}

void RoomPlainPropertyEditorSystem::OnPropSelected( std::string propName )
{
    if (!mRoomDesc.IsValid())
    {
        return;
    }
    auto propType = RoomProperty::Get()(AutoId( propName ));
    if (mRoomDesc->HasProperty( propType ))
    {
        mRoomDesc->RemoveProperty( propType );
    }
    else
    {
        mRoomDesc->AddProperty( propType );
    }
    LoadPropStates();
}

void RoomPlainPropertyEditorSystem::OnRoomEditorLoaded( map::RoomEditorLoadedEvent const& Evt )
{
    mRoomDesc = Evt.mRoomDesc;
    LoadPropStates();
}

void RoomPlainPropertyEditorSystem::LoadPropStates()
{
    if (!mRoomDesc.IsValid())
    {
        return;
    }
    mPropStates.clear();
    auto& idStorage = IdStorage::Get();
    for (auto idToProp : RoomProperty::Get().GetIdToRoomPropertyMap().left)
    {
        bool hasProp = mRoomDesc->GetPlainProperties().find( idToProp.second ) != mRoomDesc->GetPlainProperties().end();
        mPropStates.push_back( hasProp ? "YES" : "NO" );
    }
}

} // namespace map

