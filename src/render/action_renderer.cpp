#include "render/action_renderer.h"
#include "engine/cloak_system.h"
namespace render {


ActionRenderer::~ActionRenderer()
{

}

ActionRenderer::ActionRenderer( int32_t Id )
    : mId( Id )
    , mSecsToEnd( 1.0 )
    , mState( 0 )
    , mRenderableRepo( RenderableRepo::Get() )
    , mOrder( 0 )
{

}

void ActionRenderer::Init( const Actor& actor )
{
}

void ActionRenderer::FillRenderableSprites( const Actor& actor, IRenderableComponent const& renderableC, RenderableSprites_t& renderableSprites )
{

}

void ActionRenderer::Update( double DeltaTime )
{
    double nextState = mSecsToEnd == 0 ? 100 : ( mState + 1. / mSecsToEnd * DeltaTime * 100. );

    if( nextState >= 100 )
    {
        nextState = fmod( nextState, 100. );
    }
    mState = nextState;
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

} // namespace render