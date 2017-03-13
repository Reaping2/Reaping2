#include "render/weapon_action_renderer.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
namespace render {


WeaponActionRenderer::WeaponActionRenderer( int32_t Id )
    : ActionRenderer( Id )
{

}

void WeaponActionRenderer::Init( const Actor& actor )
{
    Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedItem( ItemType::Weapon );
    if ( !weapon.IsValid() )
    {
        return;
    }
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( weapon->GetId() );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}

void WeaponActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    Opt<Weapon> weapon = actor.Get<IInventoryComponent>()->GetSelectedItem( ItemType::Weapon );
    if ( !weapon.IsValid() )
    {
        return;
    }
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( weapon->GetId() );
    if( Spr.IsValid() )
    {
        SpritePhase const& Phase = Spr( ( int32_t )GetState() );
        glm::vec4 col = GetCloakColor( actor ) * GetColor( actor );
        renderableSprites.push_back( RenderableSprite( &actor, &renderableC, weapon->GetId(), &Spr, &Phase, col ) );
    }
}

} // namespace render
