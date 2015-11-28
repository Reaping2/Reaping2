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
        mValidIds.push_back( Id );
    }
}

void DamageDecals::OnDamageTaken( DamageTakenEvent const& Evt )
{
    if( mValidIds.empty() )
    {
        return;
    }
//     if( !( rand() % 3 ) )
//     {
//         return;
//     }
    Decal Part;
    Part.mCenter = Evt.Pos + glm::vec2( ( rand() % 10 - 5.f ) / 200.f, ( rand() % 10 - 5.f ) / 200.f );
    Part.mId = mValidIds[rand() % mValidIds.size()];
    mDecalEngine.Add( Part, DecalEngine::GroundParticle );
}

