#include "platform/i_platform.h"
#include "engine/controllers/controller_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/i_controller_component.h"
namespace engine {

ControllerSystem::ControllerSystem()
    : SubSystemHolder()
    , mScene( Scene::Get() )
{

}

void ControllerSystem::Init()
{
    SubSystemHolder::Init();
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<IControllerComponent>().IsValid(); } );
}


void ControllerSystem::Update( double DeltaTime )
{
    for (auto actor : mScene.GetActorsFromMap( GetType_static() ))
    {
        Opt<IControllerComponent> controllerC = actor->Get<IControllerComponent>();
        if ( !controllerC.IsValid() || !controllerC->IsEnabled() )
        {
            continue;
        }
        BindIds_t& bindIds = mSubSystems.get<SubSystemHolder::AllByBindId>();
        BindIds_t::iterator subsysIt = bindIds.find( controllerC->GetId() );
        if ( subsysIt != bindIds.end() )
        {
            subsysIt->mSystem->Update( *actor, DeltaTime );
        }
    }
}

} // namespace engine

