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
    mEditorModels.push_back( new ModelValue( IntFunc( this, &EditorTargetSystem::TargetChanged ), "target", &editorModel ) );
    ModelValue& targetModel = mEditorModels.back();
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Guns, this ), "guns", &targetModel) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Buffs, this ), "buffs", &targetModel) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_int_vec_t) boost::bind( &EditorTargetSystem::Items, this ), "items", &targetModel) );

    using namespace boost::assign;
    mGuns += AutoId("pistol"), AutoId("plasma_gun"), AutoId("rocket_launcher"), AutoId("shotgun"),AutoId("ion_gun"), AutoId("gatling_gun"), AutoId("gauss_gun");
    mBuffs += AutoId("HealOverTimeBuff"),AutoId("MoveSpeedBuff"),AutoId("AccuracyBuff"),AutoId("ArmorBuff"),AutoId("CloakBuff");
    mItems += AutoId("flash_normal_item"),AutoId("grenade_normal_item"),AutoId("cloak_normal_item"),AutoId("blue_grenade_normal_item");
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

void EditorTargetSystem::TargetChanged( int32_t target )
{
    if ( mCursor.IsValid() )
    {
        mScene.RemoveActor( mCursor->GetGUID() );
    }
    mTargetId = target;
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
    return mGuns;
}

std::vector<int32_t> EditorTargetSystem::Buffs()
{
    return mBuffs;
}

std::vector<int32_t> EditorTargetSystem::Items()
{
    return mItems;
}

} // namespace map

