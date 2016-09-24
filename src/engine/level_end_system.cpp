#include "platform/i_platform.h"
#include "level_end_system.h"
#include "core/i_level_end_component.h"

namespace engine {

LevelEndSystem::LevelEndSystem()
    : mScene( Scene::Get() )
{
}


void LevelEndSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<ILevelEndComponent>().IsValid(); } );
}


void LevelEndSystem::Update(double DeltaTime)
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
       Opt<ILevelEndComponent> levelEndC = actor->Get<ILevelEndComponent>();
       if (!levelEndC.IsValid())
       {
           continue;
       }
    }
}


} // namespace engine

