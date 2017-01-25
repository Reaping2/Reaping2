#include "render/idle_action_renderer.h"
#include "platform/auto_id.h"
#include "platform/id_storage.h"
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
    std::tie( mJointSpr, mMaskSpr ) = GetJointAndMaskSprites( actorId, aid );
}

void IdleActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    if( nullptr != mSpr )
    {
        int32_t st( GetState() );
        SpritePhase const& Phase = (*mSpr)( st );
        glm::vec4 col = GetCloakColor( actor ) * GetColor( actor );
        static int32_t aid = AutoId( "body_idle" );
        RenderableSprite rs( &actor, &renderableC, aid, mSpr, &Phase, col );
        if( nullptr != mJointSpr )
        {
            rs.AdditionalSprs.push_back( &(*mJointSpr)( st ) );
        }
        if( nullptr != mMaskSpr )
        {
            rs.MaskSpr = &(*mMaskSpr)( st );
        }
        renderableSprites.push_back( rs );
    }
}

std::pair<Sprite const*, Sprite const*> IdleActionRenderer::GetJointAndMaskSprites( int32_t actorId, int32_t actionId )
{
    auto rv = std::make_pair<Sprite const*, Sprite const*>( nullptr, nullptr );
    static RenderableRepo& rr( RenderableRepo::Get() );
    SpriteCollection const& Sprites = rr( actorId );
    SpriteCollection const& Joint = rr( Sprites.JointId() );
    std::string actorName;
    platform::IdStorage::Get().GetName( actorId, actorName );
    SpriteCollection const& Mask = rr( AutoId( actorName + "_mask" ) );

    Sprite const& JointSpr = Joint( actionId );
    Sprite const& MaskSpr = Mask( actionId );
    if( JointSpr.IsValid() )
    {
        rv.first = &JointSpr;
    }
    if( MaskSpr.IsValid() )
    {
        rv.second = &MaskSpr;
    }
    return rv;
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
