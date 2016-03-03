#include "corpses.h"
#include "renderable_repo.h"
#include "core/actor.h"
#include "core/i_position_component.h"
#include "core/i_collision_component.h"
#include <boost/bind.hpp>

Corpses::Corpses()
    : mDecalEngine( DecalEngine::Get() )
{
    mOnActorEvent = EventServer<ActorEvent>::Get().Subscribe(
            boost::bind( &Corpses::OnActorEvent, this, _1 ) );
}

void Corpses::OnActorEvent( ActorEvent const& Evt )
{
    if ( Evt.mState!=ActorEvent::Removed )
    {
        return;
    }
    Actor const& actor = *(Evt.mActor);
    Opt<IPositionComponent> posC = actor.Get<IPositionComponent>();
    if ( !posC.IsValid() )
    {
        return;
    }
    static RenderableRepo& Rends{ RenderableRepo::Get() };
    static int32_t DefaultActId = AutoId( "corpse" );
    Sprite const& Spr = Rends( actor.GetId() )( DefaultActId );
    if( !Spr.IsValid() )
    {
        return;
    }
    SpritePhase const& Phase = Spr( 0 );

    Decal Part;
    Part.mCenter = glm::vec2( posC->GetX(), posC->GetY() );
    Part.mTexId = Phase.TexId;
    Part.mTexCoords = glm::vec4{ Phase.Left, Phase.Right, Phase.Bottom, Phase.Top };

    //TODO: this one should not depend on collision radius
    Opt<ICollisionComponent> const collisionC = actor.Get<ICollisionComponent>();
    Part.mRadius = ( collisionC.IsValid() ? collisionC->GetRadius() : 50. ) * Spr.GetScale();

    Part.mHeading = posC->GetOrientation();
    Part.mAlpha = 1.0;
    mDecalEngine.Add( Part, DecalEngine::Corpse );
}

