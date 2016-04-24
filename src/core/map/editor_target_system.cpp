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
    mEditorModels.push_back( new ModelValue( StringFunc( this, &EditorTargetSystem::TargetChanged ), "target", &editorModel ) );
    ModelValue& targetModel = mEditorModels.back();
    mEditorModels.push_back( new ModelValue( (ModelValue::get_string_vec_t) boost::bind( &EditorTargetSystem::Guns, this ), "guns", &targetModel) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_string_vec_t) boost::bind( &EditorTargetSystem::Buffs, this ), "buffs", &targetModel) );
    mEditorModels.push_back( new ModelValue( (ModelValue::get_string_vec_t) boost::bind( &EditorTargetSystem::Items, this ), "items", &targetModel) );

    using namespace boost::assign;
    mGuns += "pistol", "plasma_gun", "rocket_launcher", "shotgun","ion_gun", "gatling_gun", "gauss_gun";
    mBuffs += "HealOverTimeBuff","MoveSpeedBuff","AccuracyBuff","ArmorBuff","CloakBuff";
    // TODO: these names may be to techical for the user -> cut name and append "_normal_item" later?
    mItems += "flash_normal_item","grenade_normal_item","cloak_normal_item","blue_grenade_normal_item";
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

void EditorTargetSystem::TargetChanged( std::string const& target )
{
    if ( mCursor.IsValid() )
    {
        mScene.RemoveActor( mCursor->GetGUID() );
    }
    mTargetId = AutoId( target );
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

std::vector<std::string> EditorTargetSystem::Guns()
{
    return mGuns;
}

std::vector<std::string> EditorTargetSystem::Buffs()
{
    return mBuffs;
}

std::vector<std::string> EditorTargetSystem::Items()
{
    return mItems;
}


} // namespace map

