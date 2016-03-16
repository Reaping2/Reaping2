#include "i_render.h"
#include "health_bar_renderer.h"
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
#include "shader_manager.h"
#include "font.h"
#include "uimodel.h"
#include "core/i_team_component.h"
#include "core/i_armor_component.h"
#include "engine/cloak_system.h"


void HealthBarRenderer::Init()
{
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "health_bar" );
    mWindow = engine::Engine::Get().GetSystem<engine::WindowSystem>();
    mSize = glm::vec2(mSettings.GetInt("health_bar.size.width", 150)
        , mSettings.GetInt("health_bar.size.height", 14));
    mPosition = glm::vec2(-mSize.x / 2
        , mSettings.GetInt("health_bar.y", 45) - mSize.y / 2);
    mBorderSize = glm::vec2(mSettings.GetInt("health_bar.border_size.width", 150)
        , mSettings.GetInt("health_bar.border_size.height", 14));
    mBorderPosition = glm::vec2(-mBorderSize.x / 2
        , mSettings.GetInt("health_bar.y", 45) - mBorderSize.y / 2);
}

HealthBarRenderer::HealthBarRenderer()
    : mProgramState( core::ProgramState::Get() )
    , mColorRepo( render::ColorRepo::Get() )
    , mSettings( Settings::Get() )
{
    Init();
}

void HealthBarRenderer::Draw()
{
    SceneVertices_t Vertices;
    Vertices.reserve( mPrevVertices.size() );
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions;
    Positions_t TexCoords;
    Positions.reserve( mPrevVertices.size() );
    Positions_t TextPosition;
    TextPosition.reserve( mPrevVertices.size() );
    Positions_t Dimensions;
    Dimensions.reserve( mPrevVertices.size() );

    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors;
    Colors.reserve( mPrevVertices.size() );
    SceneVertexInserter_t Inserter( Vertices );
    struct ChangedAt
    {
        size_t Start;
        size_t Count;
    };
    int32_t lastVertexIndex = 0;

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

        Opt<IHealthComponent> healthC( player->Get<IHealthComponent>() );
        Opt<ITeamComponent> teamC( player->Get<ITeamComponent>() );

        Opt<IArmorComponent> armorC = player->Get<IArmorComponent>();
        int32_t armor = 0;
        if ( armorC.IsValid() )
        {
            armor = armorC->GetCurrentArmor();
        }

        double currPercent = healthC->GetHP() / double( healthC->GetMaxHP().Get() );
        double currArmorPercent = armor / double( healthC->GetMaxHP().Get() + armor );

        bool isself = i->mClientActorGUID == mProgramState.mControlledActorGUID;
        {
            glm::vec4 dim( int32_t( positionC->GetX() ) + mBorderPosition.x, int32_t( positionC->GetY() ) + mBorderPosition.y, mBorderSize.x, mBorderSize.y );
            glm::vec4 col = isself ? glm::vec4( 0.0, 0.0, 0.0, 0.7 ) : glm::vec4( 0.0, 0.0, 0.0, 0.7 );
            ColoredBox( dim, col, Inserter );
        }
        {
            glm::vec4 dim( int32_t( positionC->GetX() ) + mPosition.x, int32_t( positionC->GetY() ) + mPosition.y, currPercent * mSize.x, mSize.y );
            glm::vec4 col = isself ? glm::vec4( 0.59, 0.15, 0.7, 1.0 ) : glm::vec4( 0.0, 0.8, 0.0, 0.7 );
            if ( teamC.IsValid() )
            {
                col = mColorRepo( teamC->GetTeam() );
            }
            ColoredBox( dim, col, Inserter );
        }
        {
            double currCombinedPercent = currPercent + currArmorPercent > 1.0 ? 1 - currArmorPercent : currPercent;
            glm::vec4 dim( int32_t( positionC->GetX() ) + mPosition.x + currCombinedPercent * mSize.x
                           , int32_t( positionC->GetY() ) + mPosition.y
                           , currArmorPercent * mSize.x, mSize.y );
            glm::vec4 col = glm::vec4( 1.0, 1.0, 1.0, 0.7 );
            ColoredBox( dim, col, Inserter );
        }

    }
    size_t const CurSize = Vertices.size();
    if ( CurSize == 0 )
    {
        return;
    }
    SceneVertex vertex = Vertices.back();

    // todo: check and track changes
    for( SceneVertices_t::const_iterator i = Vertices.begin(), e = Vertices.end(); i != e; ++i )
    {
        SceneVertex const& Vert = *i;
        Positions.push_back( Vert.Pos );
        Colors.push_back( Vert.Color );
        TextPosition.push_back( Vert.RealPos );
        //Dimensions.push_back(Vert.Dimensions);
    }


    mVAO.Bind();
    if( CurSize != mPrevVertices.size() )
    {
        size_t TotalSize = CurSize * ( sizeof( glm::vec4 ) + 3 * sizeof( glm::vec2 ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = CurSize * sizeof( glm::vec2 );
    GLuint CurrentAttribIndex = 0;

    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TextPosition[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );
    //     ++CurrentAttribIndex;
    //     CurrentOffset += CurrentSize;
    //     CurrentSize = CurSize * sizeof( glm::vec2 );
    //     glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Dimensions[0] );
    //     glEnableVertexAttribArray( CurrentAttribIndex );
    //     glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );



    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "health_bar" );
    int w, h;
    mWindow->GetWindowSize( w, h );
    glDrawArrays( GL_TRIANGLES, 0, CurSize );

    mVAO.Unbind();
    // store current match
    using std::swap;
    swap( mPrevVertices, Vertices );
    mTexts.clear();
}

void HealthBarRenderer::AddText( Text const& text )
{
    mTexts.push_back( text );
}

void HealthBarRenderer::ColoredBox( glm::vec4 const& Dim, glm::vec4 const& Col, SceneVertexInserter_t& Inserter )
{
    *Inserter++ = SceneVertex( glm::vec2( Dim.x, Dim.y ), Col, glm::vec2( 0.0, 0.0 )  );
    *Inserter++ = SceneVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), Col, glm::vec2( 0.0, 1.0 ) );
    *Inserter++ = SceneVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), Col, glm::vec2( 1.0, 0.0 ) );
    *Inserter++ = SceneVertex( glm::vec2( Dim.x + Dim.z, Dim.y ), Col, glm::vec2( 1.0, 0.0 ) );
    *Inserter++ = SceneVertex( glm::vec2( Dim.x, Dim.y + Dim.w ), Col, glm::vec2( 0.0, 1.0 ) );
    *Inserter++ = SceneVertex( glm::vec2( Dim.x + Dim.z, Dim.y + Dim.w ), Col, glm::vec2( 1.0, 1.0 ) );
}


