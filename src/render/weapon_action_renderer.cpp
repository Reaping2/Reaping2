#include "render/weapon_action_renderer.h"
#include "platform/auto_id.h"
#include "core/i_inventory_component.h"
namespace render{


    WeaponActionRenderer::WeaponActionRenderer(int32_t Id)
        : ActionRenderer(Id)
    {

    }

    void WeaponActionRenderer::Init(const Actor& actor)
    {
        Opt<Weapon> weapon=actor.Get<IInventoryComponent>()->GetSelectedWeapon();
        if (!weapon.IsValid())
        {
            return;
        }
        SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
        Sprite const& Spr=Sprites(weapon->GetId());
        if( Spr.IsValid() )
        {
            mSecsToEnd=Spr.GetSecsToEnd();
        }
    }

    void WeaponActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
    {
        Opt<Weapon> weapon=actor.Get<IInventoryComponent>()->GetSelectedWeapon();
        if (!weapon.IsValid())
        {
            return;
        }
        SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
        Sprite const& Spr=Sprites(weapon->GetId());
        if( Spr.IsValid() )
        {
            SpritePhase const& Phase = Spr( (int32_t)GetState() );
            renderableSprites.push_back( RenderableSprite( &actor, weapon->GetId(), &Spr, &Phase ) );
        }
    }

} // namespace render