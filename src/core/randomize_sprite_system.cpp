#include "platform/i_platform.h"
#include "randomize_sprite_system.h"
#include "i_renderable_component.h"
#include "i_border_component.h"

namespace engine {

RandomizeSpriteSystem::RandomizeSpriteSystem()
    : mScene( Scene::Get() )
{
}


void RandomizeSpriteSystem::Init()
{
    mOnActor=EventServer<::ActorEvent>::Get().Subscribe( boost::bind( &RandomizeSpriteSystem::OnActorEvent, this, _1 ) );
}


void RandomizeSpriteSystem::Update(double DeltaTime)
{
}

void RandomizeSpriteSystem::OnActorEvent(::ActorEvent const& Evt)
{
    if (Evt.mState != ActorEvent::Added)
    {
        return;
    }
    auto renderableC( Evt.mActor->Get<IRenderableComponent>() );
    if (!renderableC.IsValid())
    {
        return;
    }
    int32_t ind = 0;
    if (!renderableC->GetRandomSprites().empty())
    {
        ind = GetRandomIndex( renderableC->GetRandomSprites() );
    }
    renderableC->SetSpriteIndex( ind );
    
    auto borderC( Evt.mActor->Get<IBorderComponent>() );
    if (!borderC.IsValid())
    {
        return;
    }
    ind = 0;
    if (!borderC->GetRandomSprites().empty())
    {
        ind = GetRandomIndex( borderC->GetRandomSprites() );
    }
    borderC->SetSpriteIndex( ind );
    L2( "set spriteIndex %d  ", ind );
}

int32_t RandomizeSpriteSystem::GetRandomIndex( std::vector<int32_t> const& randomSprites )
{
    int32_t sumWeight = 0;
    for (auto weight : randomSprites)
    {
        sumWeight += weight;
    }
    int32_t selected = RandomGenerator::global()() % sumWeight;
    sumWeight = 0;
    int32_t ind = 0;
    for (auto weight : randomSprites)
    {
        sumWeight += weight;
        if (sumWeight > selected)
        {
            break;
        }
        ++ind;
    }
    return ind;
}


} // namespace engine

