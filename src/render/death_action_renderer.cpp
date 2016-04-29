#include "render/death_action_renderer.h"
#include "platform/auto_id.h"
namespace render {


DeathActionRenderer::DeathActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

void DeathActionRenderer::Init( const Actor& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    int32_t aid = AutoId( "death" );
    Sprite const& Spr = Sprites( aid );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void DeathActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    int32_t aid = AutoId( "death" );
    Sprite const& Spr = Sprites( aid );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, aid, &Spr, &Phase ) );
    }
}

void DeathActionRenderer::Update( double DeltaTime )
{
    double nextState = mSecsToEnd == 0 ? 100 : ( mState + 1. / mSecsToEnd * DeltaTime * 100. );

    if( nextState >= 100 )
    {
        nextState = 100.;
    }
    mState = nextState;
}

} // namespace render