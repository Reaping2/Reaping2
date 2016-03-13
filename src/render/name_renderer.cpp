#include "i_render.h"
#include "name_renderer.h"
#include "core/i_position_component.h"
#include "core/i_inventory_component.h"
#include "core/i_health_component.h"
#include "core/i_collision_component.h"
#include "core/i_renderable_component.h"
#include "core/actor.h"
#include "recognizer.h"
#include "main/window.h"
#include "engine/engine.h"
#include "text.h"
#include "text_uimodel.h"
#include "core/i_team_component.h"
#include "engine/cloak_system.h"

void NameRenderer::Init()
{
}

NameRenderer::NameRenderer()
    : mProgramState( core::ProgramState::Get() )
    , mColorRepo( render::ColorRepo::Get() )
{
}

void NameRenderer::Draw( TextSceneRenderer& textSceneRenderer )
{
    for ( core::ProgramState::ClientDatas_t::iterator i = mProgramState.mClientDatas.begin(), e = mProgramState.mClientDatas.end(); i != e; ++i )
    {
        Opt<Actor> player( Scene::Get().GetActor( ( *i ).mClientActorGUID ) );
        if ( !player.IsValid() )
        {
            continue;
        }
        engine::CloakSystem::CloakState cloakState = engine::CloakSystem::GetCloakState( *player.Get() );
        if ( cloakState == engine::CloakSystem::Invisible )
        {
            continue;
        }
        Opt<IPositionComponent> positionC = player->Get<IPositionComponent>();
        Opt<ITeamComponent> teamC( player->Get<ITeamComponent>() );
        Text text( 76.0, glm::vec4( 0, 0, 500, 500 ),
                   teamC.IsValid() ? mColorRepo( teamC->GetTeam() ) : glm::vec4( 1.0, 1.0, 1.0, 1.0 )
                   , ( *i ).mClientName, glm::vec2( positionC->GetX(), ( positionC->GetY() + 50 ) ), true );
        textSceneRenderer.AddText( text );
    }
}


