#include "normal_brush.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
namespace map {

NormalBrush::NormalBrush( int32_t Id )
    : IBrush( Id )
    , mMouseLeftPressed( false )
    , mMouseRightPressed( false )
{

}

void NormalBrush::Update( double DeltaTime )
{
    if ( !EditorTargetSystem::Get()->GetCursor().IsValid() )
    {
        return;
    }

    if ( mMouseLeftPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ) )
    {
        EditorTargetSystem::Get()->GetTarget().PutTarget( EditorTargetSystem::Get()->GetCursorPosition() );
        mMouseLeftPressed = false;
    }
    else if ( engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ) )
    {
        mMouseLeftPressed = true;
    }

    if ( mMouseRightPressed && !engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ) )
    {
        std::vector<int32_t> removeActors = GetActorsToRemove();

        for ( std::vector<int32_t>::iterator i = removeActors.begin(), e = removeActors.end(); i != e; ++i )
        {
            mScene.RemoveActor( *i );
            MapSystem::Get()->RemoveMapElement( *i );
        }
        mMouseRightPressed = false;
    }
    else if ( engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Right ) )
    {
        mMouseRightPressed = true;
    }
}

} // namespace map

