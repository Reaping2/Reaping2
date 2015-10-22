#include "render/ctf_hat_action_renderer.h"
#include "core/ctf_program_state.h"
#include "core/player_controller_component.h"

namespace render {
namespace ctf {

CtfHatActionRenderer::CtfHatActionRenderer(int32_t Id)
    : ActionRenderer(Id)
    , mColorRepo(ColorRepo::Get())
{
    mCtfHatId=AutoId("ctf_hat");
}


void CtfHatActionRenderer::Init(Actor const& actor)
{
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    Sprite const& Spr=Sprites(mCtfHatId);
    if( Spr.IsValid() )
    {
        mSecsToEnd=Spr.GetSecsToEnd();
    }
}


void CtfHatActionRenderer::FillRenderableSprites(const Actor& actor, RenderableSprites_t& renderableSprites)
{
    SpriteCollection const& Sprites=mRenderableRepo(actor.GetId());
    Sprite const& Spr=Sprites(mCtfHatId);
    if( Spr.IsValid() )
    {
        Opt<PlayerControllerComponent> playerCC=actor.Get<PlayerControllerComponent>();
        if (playerCC.IsValid())
        {
            Opt< ::ctf::ClientData> ctfClientData(::ctf::ProgramState::Get().FindClientDataByClientId(playerCC->mControllerId));
            if (ctfClientData.IsValid())
            {
                SpritePhase const& Phase = Spr( (int32_t)GetState() );
                renderableSprites.push_back(
                    RenderableSprite( &actor, mCtfHatId, &Spr, &Phase, mColorRepo(ctfClientData->mTeam) ) );
            }

        }
    }
}

} // namespace ctf
} // namespace render

