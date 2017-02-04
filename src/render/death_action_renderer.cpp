#include "render/death_action_renderer.h"
#include "platform/auto_id.h"
namespace render {

DeathActionRenderer::DeathActionRenderer( int32_t Id )
    : ActionRenderer( Id, AutoId( "death" ) )
{

}

void DeathActionRenderer::Update( double DeltaTime )
{
    double nextState = mSecsToEnd == 0 ? 100 : ( mState + DeltaTime * 100. / mSecsToEnd );
    mState = std::min( 100., nextState );
}

} // namespace render
