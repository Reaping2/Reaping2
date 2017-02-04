#include "render/action_renderer.h"
#include "engine/cloak_system.h"
#include "platform/auto_id.h"
#include "platform/id_storage.h"
#include "core/i_renderable_component.h"
namespace render {


ActionRenderer::~ActionRenderer()
{

}

ActionRenderer::ActionRenderer( int32_t Id, int32_t ActionId )
    : mId( Id )
    , mActionId( ActionId == -1 ? AutoId( "idle" ) : ActionId )
    , mSecsToEnd( 1.0 )
    , mState( 0 )
    , mRenderableRepo( RenderableRepo::Get() )
    , mOrder( 0 )
{

}

void ActionRenderer::Init( const Actor& actor )
{
    int32_t actorId = actor.GetId();
    auto renderableC( actor.Get<IRenderableComponent>() );
    actorId = GetSpriteId( renderableC->GetSpriteIndex(), actorId );

    SpriteCollection const& Sprites = mRenderableRepo( actorId );
    Sprite const& Spr = Sprites( mActionId );
    if( Spr.IsValid() )
    {
        mSpr = &Spr;
        mSecsToEnd = Spr.GetSecsToEnd();
    }
    mCompanionSprites = GetCompanionSprites( actorId, mActionId );
}

glm::vec4 ActionRenderer::GetRenderableColor( Actor const& actor ) const
{
    auto const& col = GetCloakColor( actor ) * GetColor( actor );
    return col;
}
void ActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    if( nullptr == mSpr )
    {
        return;
    }
    int32_t st( GetState() );
    auto const& Phase = (*mSpr)( st );
    auto const& col = GetRenderableColor( actor );
    RenderableSprite rs( &actor, &renderableC, mActionId, mSpr, &Phase, col );
    FillRenderableSprite( rs, mCompanionSprites, st );
    renderableSprites.push_back( rs );
}

void FillRenderableSprite( RenderableSprite& rs, CompanionSprites const& data, int32_t st )
{
    for( auto const& spr : data.AdditionalSprs )
    {
        rs.AdditionalSprs.push_back( &(*spr)( st ) );
    }
    if( nullptr != data.MaskSpr )
    {
        rs.MaskSpr = &(*data.MaskSpr)( st );
    }
    if( nullptr != data.NormalSpr )
    {
        rs.NormalSpr = &(*data.NormalSpr)( st );
    }
}

void ActionRenderer::Update( double DeltaTime )
{
    double nextState = mSecsToEnd == 0 ? 100 : ( mState + DeltaTime / mSecsToEnd * 100. );

    if( nextState >= 100 )
    {
        nextState = fmod( nextState, 100. );
    }
    mState = nextState;
}


double ActionRenderer::GetState() const
{
    return mState;
}


void ActionRenderer::SetState( double S )
{
    mState = S;
}


int32_t ActionRenderer::GetId() const
{
    return mId;
}


int32_t ActionRenderer::GetOrder() const
{
    return mOrder;
}


void ActionRenderer::SetOrder( int32_t order )
{
    mOrder = order;
}

bool ActionRenderer::operator<( const render::ActionRenderer& r ) const
{
    return mOrder < r.GetOrder();
}


DefaultActionRenderer::DefaultActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

glm::vec4 GetCloakColor( const Actor& actor )
{
    glm::vec4 r = glm::vec4( 1.0 );
    engine::CloakSystem::CloakState cloakState = engine::CloakSystem::GetCloakState( actor );
    if ( cloakState == engine::CloakSystem::Cloaked )
    {
        r = glm::vec4( 1.0, 1.0, 1.0, 0.5 );
    }
    else if ( cloakState == engine::CloakSystem::Invisible )
    {
        r = glm::vec4( 0.0 );
    }
    return r;
}

glm::vec4 GetColor( const Actor& actor )
{
    auto renderableC = actor.Get<IRenderableComponent>();
    if (renderableC.IsValid())
    {
        return renderableC->GetColor();
    }
    return glm::vec4( 1.0 );
}

CompanionSprites GetCompanionSprites( int32_t actorId, int32_t actionId )
{
    CompanionSprites rv;
    static auto& renderableRepo( RenderableRepo::Get() );
    SpriteCollection const& Sprites = renderableRepo( actorId );
    SpriteCollection const& Joint = renderableRepo( Sprites.JointId() );
    std::string actorName;
    platform::IdStorage::Get().GetName( actorId, actorName );
    SpriteCollection const& Mask = renderableRepo( AutoId( actorName + "_mask" ) );
    SpriteCollection const& Normal = renderableRepo( AutoId( actorName + "_normal" ) );

    Sprite const& JointSpr = Joint( actionId );
    Sprite const& MaskSpr = Mask( actionId );
    Sprite const& NormalSpr = Normal( actionId );
    if( JointSpr.IsValid() )
    {
        rv.AdditionalSprs.push_back( &JointSpr );
    }
    if( MaskSpr.IsValid() )
    {
        rv.MaskSpr = &MaskSpr;
    }
    if( NormalSpr.IsValid() )
    {
        rv.NormalSpr = &NormalSpr;
    }
    return rv;
}

int32_t GetSpriteId( int32_t spriteIndex, int32_t actorId )
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

void DefaultActionRendererLoader::BindValues()
{

}

} // namespace render
