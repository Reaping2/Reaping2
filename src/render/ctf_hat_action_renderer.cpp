#include "render/ctf_hat_action_renderer.h"
#include "core/ctf_program_state.h"
#include "core/player_controller_component.h"
#include "core/i_team_component.h"

namespace render {
namespace ctf {

CtfHatActionRenderer::CtfHatActionRenderer( int32_t Id )
    : ActionRenderer( Id )
    , mColorRepo( ColorRepo::Get() )
{
    mCtfHatId = AutoId( "ctf_body_color" );
}


void CtfHatActionRenderer::Init( Actor const& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfHatId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}


void CtfHatActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCtfHatId );
    if( Spr.IsValid() )
    {
        //         Opt<PlayerControllerComponent> playerCC=actor.Get<PlayerControllerComponent>();
        //         if (playerCC.IsValid())
        //         {
        //             Opt< ::ctf::ClientData> ctfClientData(::ctf::ProgramState::Get().FindClientDataByClientId(playerCC->mControllerId));
        //             if (ctfClientData.IsValid())
        //             {
        Opt<ITeamComponent> teamC( actor.Get<ITeamComponent>() );
        if ( teamC.IsValid() )
        {
            SpritePhase const& Phase = Spr( ( int32_t )GetState() );
            glm::vec4 col = ColorRepo::Get()( teamC->GetTeam() );
            col.a = GetCloakColor( actor ).a;
            renderableSprites.push_back(
                RenderableSprite( &actor, &renderableC, mCtfHatId, &Spr, &Phase, col ) );
        }
        //             }
        //
        //         }
    }
}

} // namespace ctf
} // namespace render

