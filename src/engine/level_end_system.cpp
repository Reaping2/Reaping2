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
}


void LevelEndSystem::Update(double DeltaTime)
{
    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
    {
       Actor& actor = **it;
       Opt<ILevelEndComponent> levelEndC=actor.Get<ILevelEndComponent>();
       if (!levelEndC.IsValid())
       {
           continue;
       }
    }
}


} // namespace engine

