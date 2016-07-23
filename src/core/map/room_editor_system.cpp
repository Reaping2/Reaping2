#include "platform/i_platform.h"
#include "room_editor_system.h"
#include "engine/engine.h"
#include "engine/controllers/controller_system.h"
#include "ui/ui.h"
#include "json/json.h"
#include "map_system.h"
#include "spawn_actor_map_element.h"
#include "editor_grid_system.h"
#include "editor_target_system.h"
#include "editor_brush_system.h"
#include "engine/collision_system.h"
#include "ctf_soldier_spawn_point_map_element.h"
#include "respawn_actor_map_element.h"
#include "respawn_actor_map_element_system.h"
#include "ctf_flag_spawn_point_map_element.h"
#include "../i_renderable_component.h"
#include "input/keyboard_adapter_system.h"
#include <boost/assign/std/vector.hpp>
#include "level_generator/room_repo.h"
#include "level_generator/level_generated_event.h"
#include "spawn_actor_map_element_system.h"
#include "map_start_map_element.h"
#include "spawn_soldiers_map_element.h"
#include "level_generator/level_generated_map_element.h"
#include "link_map_element.h"
#include "level_generator/json_room.h"
#include "editor_back_event.h"
#include "editor_system.h"
#include "editor_mode_changed_event.h"
#include "editor_hud_state.h"
#include "room_cell_editor_system.h"

namespace map {

RoomEditorSystem::RoomEditorSystem()
    : mScene( Scene::Get() )
    , mEditorModel( "room_editor", &RootModel::Get() )
    , mStartModel( VoidFunc( this, &RoomEditorSystem::Start ), "start", &mEditorModel )
    , mLoadModel( StringFunc( this, &RoomEditorSystem::Load ), "load", &mEditorModel )
    , mModeModel( StringFunc( this, &RoomEditorSystem::ModeSelect ), "mode", &mEditorModel )
    , mSaveModel( VoidFunc( this, &RoomEditorSystem::Save ), "save", &mEditorModel )
    , mLevelModel( (ModelValue::get_string_vec_t) boost::bind(&RoomEditorSystem::LevelNames, this),"levels", &mEditorModel )
    , mX( 0 )
    , mY( 0 )
    , mEditorMode()
    , mCurrentMovement( 0 )
    , mRoomName()
    , mTimer()
    , mAutoSaveOn( false )
{
    mTimer.SetFrequency( 25000 );
}

void RoomEditorSystem::Init()
{
    mKeyboard =::engine::Engine::Get().GetSystem<engine::KeyboardSystem>();
    mOnScreenMouseMove = EventServer< ::ScreenMouseMoveEvent>::Get().Subscribe( boost::bind( &RoomEditorSystem::OnScreenMouseMove, this, _1 ) );
    mWindow = engine::Engine::Get().GetSystem<engine::WindowSystem>();
    mRenderer = engine::Engine::Get().GetSystem<engine::RendererSystem>();
    mKeyId = EventServer<KeyEvent>::Get().Subscribe( boost::bind( &RoomEditorSystem::OnKeyEvent, this, _1 ) );
    mOnEditorBack = EventServer<map::EditorBackEvent>::Get().Subscribe( boost::bind( &RoomEditorSystem::OnEditorBack, this, _1 ) );
    using namespace boost::assign;
    mLevelNames += "test_room", "test_simple_room";
}

void RoomEditorSystem::OnEditorBack( map::EditorBackEvent const& Evt )
{
    if (mEnabled)
    {
        if (Evt.mBackToBaseHud)
        {
            Ui::Get().Load( "room_editor_base_hud" );
            EditorHudState::Get().SetHudShown( false );
        }
    }
}


void RoomEditorSystem::Start()
{
    ::engine::Engine::Get().SetEnabled< ::engine::ControllerSystem>( false );
    ::engine::Engine::Get().SetEnabled< ::engine::CollisionSystem>( false );
    ::engine::Engine::Get().SetEnabled< EditorSystem>( false );
    ::engine::Engine::Get().SetEnabled< RoomEditorSystem>( true );
    RespawnActorMapElementSystem::Get()->SetRespawnOnDeath( false ); //to be able to delete target actors
    EditorTargetSystem::Get()->SetNextUID( AutoId( "spawn_at_level_generated" ) );
}

void RoomEditorSystem::Load( std::string const& room )
{
    mRoomName = room;
    mX = 0;
    mY = 0;
    ModelValue& PlayerModel = const_cast<ModelValue&>( RootModel::Get()["player"] );
    mPlayerModels.clear();
    mPlayerModels.push_back( new ModelValue( GetDoubleFunc( this, &RoomEditorSystem::GetX ), "x", &PlayerModel ) );
    mPlayerModels.push_back( new ModelValue( GetDoubleFunc( this, &RoomEditorSystem::GetY ), "y", &PlayerModel ) );

    mRoomId = AutoId( room );
    auto& aroom = RoomRepo::Get()( mRoomId );
    mRoomDesc = aroom.GetRoomDesc();
    RoomCellEditorSystem::Get()->SetRoomDesc( &mRoomDesc );
    mScene.Load( "room_editor" );
    Opt<engine::System> spawnActorMES( engine::Engine::Get().GetSystem<SpawnActorMapElementSystem>() );
    spawnActorMES->Update( 0 );
    for (MapElementList_t::iterator it = MapSystem::Get()->GetMapElementList().begin(); it != MapSystem::Get()->GetMapElementList().end(); )
    {
        if ((*it)->GetType() == SpawnActorMapElement::GetType_static())
        {
            delete (*it).Get();
            it = MapSystem::Get()->GetMapElementList().erase( it );
        }
        else
        {
            ++it;
        }
    }

    aroom.Generate( mRoomDesc, glm::vec2(0), true );
    Ui::Get().Load( "room_editor_base_hud" );
    mAutoSaveOn = true;
    EventServer<LevelGeneratedEvent>::Get().SendEvent( LevelGeneratedEvent() );
}

double const& RoomEditorSystem::GetX() const
{
    return mX;
}

double const& RoomEditorSystem::GetY() const
{
    return mY;
}

RoomEditorSystem::~RoomEditorSystem()
{
    mPlayerModels.clear();
}



void RoomEditorSystem::Update( double DeltaTime )
{
    mTimer.Update( DeltaTime );
    if ( mAutoSaveOn && mTimer.IsTime() )
    {
        Save();
    }
    glm::vec2 cameraCenter = mRenderer->GetCamera().GetCenter();
    mX = cameraCenter.x;
    mY = cameraCenter.y;
    uint32_t currentKeyMovement = 0;
    if( mKeyboard->GetKey( GLFW_KEY_W ).State == KeyState::Down )
    {
        currentKeyMovement |= engine::KeyboardAdapterSystem::MF_Up;
    }
    if( mKeyboard->GetKey( GLFW_KEY_A ).State == KeyState::Down )
    {
        currentKeyMovement |= engine::KeyboardAdapterSystem::MF_Left;
    }
    if( mKeyboard->GetKey( GLFW_KEY_S ).State == KeyState::Down )
    {
        currentKeyMovement |= engine::KeyboardAdapterSystem::MF_Down;
    }
    if( mKeyboard->GetKey( GLFW_KEY_D ).State == KeyState::Down )
    {
        currentKeyMovement |= engine::KeyboardAdapterSystem::MF_Right;
    }
    currentKeyMovement |= mCurrentMovement;
    if ( !EditorHudState::Get().IsHudShown() )
    {
        mX += 1000 * DeltaTime * ( ( ( currentKeyMovement & engine::KeyboardAdapterSystem::MF_Left ) ? -1 : 0 ) + ( ( currentKeyMovement & engine::KeyboardAdapterSystem::MF_Right ) ? 1 : 0 ) );
        mY += 1000 * DeltaTime * ( ( ( currentKeyMovement & engine::KeyboardAdapterSystem::MF_Up ) ? 1 : 0 ) + ( ( currentKeyMovement & engine::KeyboardAdapterSystem::MF_Down ) ? -1 : 0 ) );
    }
    if (mKeyboard->GetKey( GLFW_KEY_M ).State == KeyState::Typed)
    {
        if (EditorHudState::Get().IsHudShown())
        {
            EventServer<EditorBackEvent>::Get().SendEvent( EditorBackEvent( true ) );
        }
        else
        {
            EditorHudState::Get().SetHudShown( true );
            Ui::Get().Load( "room_editor_hud" );
            EventServer<EditorModeChangedEvent>::Get().SendEvent( EditorModeChangedEvent( "mode_select" ) );
        }
    }
}
void RoomEditorSystem::OnScreenMouseMove( ::ScreenMouseMoveEvent const& Evt )
{
    int w, h;
    mWindow->GetWindowSize( w, h );
    mCurrentMovement = 0;
    if( Evt.Pos.y < 100 )
    {
        mCurrentMovement |= engine::KeyboardAdapterSystem::MF_Up;
    }
    if( Evt.Pos.x < 100 )
    {
        mCurrentMovement |= engine::KeyboardAdapterSystem::MF_Left;
    }
    if( Evt.Pos.y > h - 150 )
    {
        mCurrentMovement |= engine::KeyboardAdapterSystem::MF_Down;
    }
    if( Evt.Pos.x > w - 100 )
    {
        mCurrentMovement |= engine::KeyboardAdapterSystem::MF_Right;
    }
}

void RoomEditorSystem::Save()
{
    {
        Json::Value Root( Json::arrayValue );
        Json::Value RoomObj( Json::objectValue );
        Json::Value MapElementArray( Json::arrayValue );

        for ( auto& mapElement : MapSystem::Get()->GetMapElementList() )
        {
            if (mapElement->GetType() != MapStartMapElement::GetType_static()
                 && mapElement->GetType() != LevelGeneratedMapElement::GetType_static()
                 && mapElement->GetType() != LinkMapElement::GetType_static()
                 && mapElement->GetType() != SpawnSoldiersMapElement::GetType_static() )
            {
                Json::Value Element( Json::objectValue );
                mapElement->Save( Element );
                if ( Element.size() > 0 )
                {
                    MapElementArray.append( Element );
                }
            }
        }
        RoomObj["map_elements"] = MapElementArray;
        auto& jsonRoom = static_cast<JsonRoom&>(RoomRepo::Get()(mRoomId));
        jsonRoom.Save( RoomObj, mRoomDesc );
        Root.append( RoomObj );
        {
            Json::StyledWriter Writer;
            std::string const& JString = Writer.write( Root );
            {
//                OsFile OutJson( "data/map/" + mRoomName + "/saved.json", std::ios_base::out );
                OsFile OutJson( "data/rooms/test_save/test_save.json", std::ios_base::out );
                OutJson.Write( JString );
            }
        }
    }
}

void RoomEditorSystem::OnKeyEvent( const KeyEvent& Event )
{
    if (!mEnabled)
    {
        return;
    }
}

Opt<RoomEditorSystem> RoomEditorSystem::Get()
{
    return engine::Engine::Get().GetSystem<RoomEditorSystem>();
}

std::vector<std::string> RoomEditorSystem::LevelNames()
{
    return mLevelNames;
}

void RoomEditorSystem::ModeSelect( std::string const& mode )
{
    mEditorMode = mode;
    EventServer<EditorModeChangedEvent>::Get().SendEvent( EditorModeChangedEvent( mode ) );
}


} // namespace map

