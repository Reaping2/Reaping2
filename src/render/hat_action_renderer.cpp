#include "render/hat_action_renderer.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
#include "core/player_controller_component.h"
namespace render {


glm::vec4 const ColorRepo::mDefaultColor = glm::vec4( 1, 1, 1, 1 );


ColorRepo::ColorRepo()
    : Repository<glm::vec4>( mDefaultColor )
{
    int32_t i = 0;
    mElements.insert( i, new glm::vec4( 0.8, 0.0, 0.0, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.0, 0.8, 0.8, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.9, 0.9, 0.0, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.0, 0.7, 0.0, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.8, 0.0, 0.8, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.3, 0.3, 0.3, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.95, 0.95, 0.95, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 1.0, 0.6, 0.0, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.5, 0.8, 0.5, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.6, 0.0, 1.0, 1 ) );
    ++i;
    mElements.insert( i, new glm::vec4( 0.2, 0.4, 0.8, 1 ) );
    ++i;
}

HatActionRenderer::HatActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{
    mHatId = AutoId( "body_color" );
}

void HatActionRenderer::Init( const Actor& actor )
{
    Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedWeapon();
    if ( !weapon.IsValid() )
    {
        return;
    }
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mHatId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void HatActionRenderer::FillRenderableSprites( const Actor& actor, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mHatId );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        Opt<PlayerControllerComponent> playerCC = actor.Get<PlayerControllerComponent>();
        glm::vec4 col = playerCC.IsValid() ? ColorRepo::Get()( playerCC->mControllerId ) : glm::vec4( 1, 1, 1, 1 );
        col.a = GetCloakColor( actor ).a;
        renderableSprites.push_back(
            RenderableSprite( &actor, mHatId, &Spr, &Phase, col ) );
    }
}



} // namespace render
