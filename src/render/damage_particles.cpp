#include "i_render.h"
#include "core/damage_taken_event.h"
#include "damage_particles.h"
#include "renderable_repo.h"
#include "particle.h"

using core::DamageTakenEvent;

DamageDecals::DamageDecals()
    : mDecalEngine( DecalEngine::Get() )
{
    Load();
    mOnDamageTaken = EventServer<DamageTakenEvent>::Get().Subscribe( boost::bind( &DamageDecals::OnDamageTaken, this, _1 ) );
}

void DamageDecals::Load()
{
    RenderableRepo& Rends( RenderableRepo::Get() );
    size_t c = 0;
    while( true )
    {
        int32_t Id = AutoId( "blood_particle" + boost::lexical_cast<std::string>( c++ ) );
        if( !Rends.HasElem( Id ) )
        {
            break;
        }
        static int32_t DefaultActId = AutoId( "default_action" );
        Sprite const& Spr = Rends( Id )( DefaultActId );
        if( !Spr.IsValid() )
        {
            continue;
        }
        SpritePhase const& Phase = Spr( 0 );
        mDecalDescs.emplace_back( DamageDecal { Phase.TexId, glm::vec4{ Phase.Left, Phase.Right, Phase.Bottom, Phase.Top } } );
    }
}

void DamageDecals::OnDamageTaken( DamageTakenEvent const& Evt )
{
    if( mDecalDescs.empty() )
    {
        return;
    }

    if ( Evt.type == DamageTakenEvent::Health )
    {
        Decal Part;
        Part.mCenter = Evt.Pos + glm::vec2( ( RandomGenerator::global()() % 10 - 5.f ) / 200.f, ( rand() % 10 - 5.f ) / 200.f );
        DamageDecal const& decal = mDecalDescs[RandomGenerator::global()() % mDecalDescs.size()];
        Part.mTexId = decal.TexId;
        Part.mTexCoords = decal.TexCoords;
        Part.mRadius = RandomGenerator::global()() % 2000 / 10.;
        Part.mHeading = RandomGenerator::global()() % 360 / 180. * 3.141592654;
        Part.mAlpha = RandomGenerator::global()() % 100 / 100.;
        mDecalEngine.Add( Part, DecalEngine::GroundParticle );
    }
}

