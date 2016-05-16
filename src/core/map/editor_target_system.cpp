#include "platform/i_platform.h"
#include "editor_target_system.h"
#include "engine/engine.h"
#include "../i_position_component.h"
#include "ui/ui.h"
#include "../i_collision_component.h"
#include <boost/assign/std/vector.hpp>

namespace map {

EditorTargetSystem::EditorTargetSystem()
    : mScene( Scene::Get() )
    , mTargetRepo( TargetRepo::Get() )
    , mTargetId( -1 )
    , mCursorPosition( 0.0, 0.0 )
    , mCursor( NULL )
{
}


void EditorTargetSystem::Init()
{
    ModelValue& editorModel = const_cast<ModelValue&>( RootModel::Get()["editor"] );
    mEditorModels.push_back( new ModelValue( "target", &editorModel ) );
    ModelValue& targetModel = mEditorModels.back();
    mEditorModels.push_back( new ModelValue( "pickups", &targetModel) );
    ModelValue& pickupModel = mEditorModels.back();
    // for the menus
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Guns, this ), "guns", &pickupModel ) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Buffs, this ), "buffs", &pickupModel ) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Items, this ), "items", &pickupModel ) );

    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::MapItems, this ), "mapitems", &targetModel ) );

    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Spawnpoints, this ), "spawnpoints", &targetModel ) );
    // for the menu actions
    mEditorModels.push_back( new ModelValue( IntFunc( this, boost::bind(&EditorTargetSystem::TargetChanged,this,"spawnpoint",_2) ), "spawntarget", &editorModel ) );
    mEditorModels.push_back( new ModelValue( IntFunc( this, boost::bind(&EditorTargetSystem::TargetChanged,this,"mapitem",_2) ), "mapitemtarget", &editorModel ) );
    mEditorModels.push_back( new ModelValue( IntFunc( this, boost::bind(&EditorTargetSystem::TargetChanged,this,"gun",_2) ), "guntarget", &editorModel ) );
    mEditorModels.push_back( new ModelValue( IntFunc( this, boost::bind(&EditorTargetSystem::TargetChanged,this,"buff",_2) ), "bufftarget", &editorModel ) );
    mEditorModels.push_back( new ModelValue( IntFunc( this, boost::bind(&EditorTargetSystem::TargetChanged,this,"item",_2) ), "itemtarget", &editorModel ) );

    // mapping the visual ids to the actor ids
    using namespace boost::assign;
    mGunVisualIds += AutoId("pistol"), AutoId("plasma_gun"), AutoId("rocket_launcher"), AutoId("shotgun"),AutoId("ion_gun"), AutoId("gatling_gun"), AutoId("gauss_gun");
    mGunActorIds = mGunVisualIds;

    mBuffVisualIds += AutoId("HealOverTimeBuff"),AutoId("MoveSpeedBuff"),AutoId("AccuracyBuff"),AutoId("ArmorBuff"),AutoId("CloakBuff");
    mBuffActorIds = mBuffVisualIds;

    mItemVisualIds += AutoId("flash_normal_item"),AutoId("grenade_normal_item"),AutoId("cloak_normal_item"),AutoId("blue_grenade_normal_item");
    mItemActorIds = mItemVisualIds;

    mMapitemVisualIds += AutoId("wall"), AutoId("wall_small"), AutoId("stone_wall"), AutoId("water"), AutoId("grass_tile"), AutoId("concrete");
    mMapitemActorIds = mMapitemVisualIds;

    mSpawnpointVisualIds += mTargetRepo( AutoId("ctf_flag_spawn_red")).GetCursorId(), mTargetRepo(AutoId("ctf_flag_spawn_blue")).GetCursorId(), mTargetRepo(AutoId("ctf_soldier_spawn_blue")).GetCursorId(), mTargetRepo(AutoId("ctf_soldier_spawn_red")).GetCursorId();
    mSpawnpointActorIds += AutoId("ctf_flag_spawn_red"), AutoId("ctf_flag_spawn_blue"), AutoId("ctf_soldier_spawn_blue"), AutoId("ctf_soldier_spawn_red");

}

void EditorTargetSystem::Update( double DeltaTime )
{
    GetTarget().Update( DeltaTime );
    if ( mCursor.IsValid() )
    {
        Opt<IPositionComponent> positionC( mCursor->Get<IPositionComponent>() );
        if ( positionC.IsValid() )
        {
            positionC->SetX( mCursorPosition.x );
            positionC->SetY( mCursorPosition.y );
        }
    }
}

void EditorTargetSystem::TargetChanged( std::string const& targetType, int32_t targetIdx )
{
    if ( mCursor.IsValid() )
    {
        mScene.RemoveActor( mCursor->GetGUID() );
    }
    if ( "spawnpoint" == targetType )
    {
        mTargetId = mSpawnpointActorIds[targetIdx];
    }
    else if ( "mapitem" == targetType )
    {
        mTargetId = mMapitemActorIds[targetIdx];
    }
    else if ( "gun" == targetType )
    {
        mTargetId = mGunActorIds[targetIdx];
    }
    else if ( "buff" == targetType )
    {
        mTargetId = mBuffActorIds[targetIdx];
    }
    else if ( "item" == targetType )
    {
        mTargetId = mItemActorIds[targetIdx];
    }
    else
    {
        return;
    }
    std::auto_ptr<Actor> cursor( GetTarget().GetCursor() );
    Opt<IPositionComponent> positionC( cursor->Get<IPositionComponent>() );
    if ( positionC.IsValid() )
    {
        positionC->SetX( mCursorPosition.x );
        positionC->SetY( mCursorPosition.y );
    }
    int32_t guid = cursor->GetGUID();
    mScene.AddActor( cursor.release() );
    mCursor = mScene.GetActor( guid );
    Ui::Get().Load( "editor_hud" );
}

EditorTargetSystem::~EditorTargetSystem()
{
    mEditorModels.clear();
}

Opt<EditorTargetSystem> EditorTargetSystem::Get()
{
    return engine::Engine::Get().GetSystem<EditorTargetSystem>();
}

ITarget& EditorTargetSystem::GetTarget()
{
    return mTargetRepo( mTargetId );
}

glm::vec2 EditorTargetSystem::GetCursorPosition() const
{
    return mCursorPosition;
}

void EditorTargetSystem::SetCursorPosition( double x, double y )
{
    mCursorPosition.x = x;
    mCursorPosition.y = y;
}

Opt<Actor> EditorTargetSystem::GetCursor() const
{
    return mCursor;
}

double EditorTargetSystem::GetCursorRadius() const
{
    double r = 1.0;
    Opt<Actor> cursor( GetCursor() );
    if ( !cursor.IsValid() )
    {
        return r;
    }
    Opt<ICollisionComponent> collisionC( cursor->Get<ICollisionComponent>() );
    if ( !collisionC.IsValid() )
    {
        return r;
    }
    return collisionC->GetRadius();
}

std::vector<int32_t> EditorTargetSystem::Guns()
{
    return mGunVisualIds;
}

std::vector<int32_t> EditorTargetSystem::Buffs()
{
    return mBuffVisualIds;
}

std::vector<int32_t> EditorTargetSystem::Items()
{
    return mItemVisualIds;
}

std::vector<int32_t> EditorTargetSystem::MapItems()
{
    return mMapitemVisualIds;
}

std::vector<int32_t> EditorTargetSystem::Spawnpoints()
{
    return mSpawnpointVisualIds;
}

} // namespace map

