#include "render/idle_action_renderer.h"
#include "platform/auto_id.h"
namespace render {


IdleActionRenderer::IdleActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

void IdleActionRenderer::Init( const Actor& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    static int32_t aid = AutoId( "body_idle" );
    Sprite const& Spr = Sprites( aid );
    if( Spr.IsValid() )
    {
        mSpr = &Spr;
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void IdleActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    if( nullptr != mSpr )
    {
        SpritePhase const& Phase = (*mSpr)( ( int32_t )GetState() );
        glm::vec4 col = GetCloakColor( actor );
        static int32_t aid = AutoId( "body_idle" );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, aid, mSpr, &Phase, col ) );
    }
}

} // namespace render
