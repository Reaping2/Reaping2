#include "render/soldier_current_action_renderer.h"
#include "core/ctf_program_state.h"
#include "core/player_controller_component.h"
#include "core/i_team_component.h"
#include "core/program_state.h"

namespace render {
namespace ctf {

SoldierCurrentActionRenderer::SoldierCurrentActionRenderer( int32_t Id )
    : ActionRenderer( Id )
    , mColorRepo( ColorRepo::Get() )
    , mColor( 0.29, 0.05, 0.4, 0.8 )
{
    mCurrentId = AutoId( "soldier_current" );
}


void SoldierCurrentActionRenderer::Init( Actor const& actor )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCurrentId );
    if( Spr.IsValid() )
    {
        mSecsToEnd = Spr.GetSecsToEnd();
    }
}


void SoldierCurrentActionRenderer::FillRenderableSprites( const Actor& actor, RenderableSprites_t& renderableSprites )
{
    SpriteCollection const& Sprites = mRenderableRepo( actor.GetId() );
    Sprite const& Spr = Sprites( mCurrentId );
    if( Spr.IsValid() )
    {
        //         Opt<PlayerControllerComponent> playerCC=actor.Get<PlayerControllerComponent>();
        //         if (playerCC.IsValid())
        //         {
        //             Opt< ::ctf::ClientData> ctfClientData(::ctf::ProgramState::Get().FindClientDataByClientId(playerCC->mControllerId));
        //             if (ctfClientData.IsValid())
        //             {
        //        Opt<ITeamComponent> teamC(actor.Get<ITeamComponent>());
        if ( actor.GetGUID() == core::ProgramState::Get().mControlledActorGUID )
            //        if (teamC.IsValid())
        {
            SpritePhase const& Phase = Spr( ( int32_t )GetState() );
            renderableSprites.push_back(
                RenderableSprite( &actor, mCurrentId, &Spr, &Phase, mColor ) );
        }
        //             }
        //
        //         }
    }
}

} // namespace ctf
} // namespace render

