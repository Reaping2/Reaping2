#include "platform/i_platform.h"
#include "worm_body_system.h"
#include "core/i_worm_body_component.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include "core/i_move_component.h"

namespace engine {

WormBodySystem::WormBodySystem()
    : mScene( Scene::Get() )
{
}


void WormBodySystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IWormBodyComponent>().IsValid(); } );
}


void WormBodySystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        auto wormBodyC( actor->Get<IWormBodyComponent>() );
        if (!wormBodyC.IsValid())
        {
            continue;
        }
    }
}


} // namespace engine

