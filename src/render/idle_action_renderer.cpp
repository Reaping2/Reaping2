#include "render/idle_action_renderer.h"
#include "platform/auto_id.h"
#include "core/i_renderable_component.h"
namespace render {


IdleActionRenderer::IdleActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

void IdleActionRenderer::Init( const Actor& actor )
{
    int32_t actorId = actor.GetId();
    auto renderableC( actor.Get<IRenderableComponent>() );
    actorId = GetSpriteId( renderableC->GetSpriteIndex(), actorId );
    
    SpriteCollection const& Sprites = mRenderableRepo( actorId );
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
        glm::vec4 col = GetCloakColor( actor ) * GetColor( actor );
        static int32_t aid = AutoId( "body_idle" );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, aid, mSpr, &Phase, col ) );
    }
}

int32_t IdleActionRenderer::GetSpriteId( int32_t spriteIndex, int32_t actorId )
{
    if (spriteIndex > 0)
    {
        static auto& idStorage = IdStorage::Get();
        std::string actorName;
        bool const gotId = idStorage.GetName( actorId, actorName );
        BOOST_ASSERT( gotId );
        actorName = actorName + "_" + std::to_string( spriteIndex );
        actorId = idStorage.GetId( actorName );
    }        
    return actorId;
}

} // namespace render
