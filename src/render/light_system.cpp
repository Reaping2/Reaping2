#include "platform/i_platform.h"
#include "light_system.h"
#include "core/i_light_component.h"
#include "core/i_position_component.h"
#include "engine/system_factory.h"
#include "core/program_state.h"
#include "core/actor_factory.h"

namespace render {

LightSystem::LightSystem()
    : mScene( Scene::Get() )
{
}


void LightSystem::Init()
{
    mScene.AddValidator( GetType_static(), []( Actor const& actor )->bool {
        return actor.Get<ILightComponent>().IsValid()
            && actor.Get<IPositionComponent>().IsValid(); } );
}


void LightSystem::Update(double DeltaTime)
{
    static core::ProgramState& ps( core::ProgramState::Get() );
    auto player = mScene.GetActor( ps.mControlledActorGUID );
    if( !player.IsValid() )
    {
        return;
    }
    auto playerPosC = player->Get<IPositionComponent>();
    if( !playerPosC.IsValid() )
    {
        return;
    }
    std::vector<Actor*> activeLights;
    for( auto actor : mScene.GetActorsFromMap( GetType_static() ) )
    {
        auto posC=actor->Get<IPositionComponent>();
        auto lightC=actor->Get<ILightComponent>();
        glm::vec2 dist( posC->GetX() - playerPosC->GetX(),
               posC->GetY() - playerPosC->GetY() );
        auto distSq = glm::dot( dist, dist );
        auto r = lightC->GetRadius() + 2000.0;
        if( distSq <= r*r*2 )
        {
            activeLights.push_back( actor );
        }
    }

    mActiveDummyObjects.resize( std::max( mActiveDummyObjects.size(), activeLights.size() ) );
    auto lightIt = activeLights.begin();
    for( auto*& dummy : mActiveDummyObjects )
    {
        if( activeLights.end() == lightIt )
        {
            // drop dummmy
            if( dummy != nullptr )
            {
                mScene.RemoveActor( dummy->GetGUID() );
            }
            continue;
        }
        if( dummy == nullptr )
        {
            static ActorFactory& af( ActorFactory::Get() );
            auto newDummy( af( AutoId( "light_active_dummy" ) ) );
            dummy = newDummy.get();
            mScene.AddActor( newDummy.release() );
        }
        auto light = *lightIt;
        dummy->Get<IPositionComponent>()->SetX( light->Get<IPositionComponent>()->GetX() );
        dummy->Get<IPositionComponent>()->SetY( light->Get<IPositionComponent>()->GetY() );
        ++lightIt;
    }

    mActiveDummyObjects.resize( activeLights.size() );
    std::swap( activeLights, mActiveLights );
}

std::vector<Actor*> const& LightSystem::GetActiveLights() const
{
    return mActiveLights;
}

REGISTER_SYSTEM( LightSystem );

} // namespace render

