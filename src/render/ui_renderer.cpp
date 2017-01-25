#include "i_render.h"
#include "ui_renderer.h"
#include "main/window.h"
#include "engine/engine.h"
#include "uimodel_repo.h"
#include "shader_manager.h"
#include "uimodel.h"
#include "font.h"
#include "counter.h"
#include "platform/game_clock.h"

namespace {
typedef std::vector<glm::vec2> Positions_t;
typedef std::vector<glm::vec4> Colors_t;
bool getNextTextId( UiVertices_t::const_iterator& i, UiVertices_t::const_iterator e,
                    Positions_t& Positions, Colors_t& Colors, Positions_t& TexCoords,
                    render::RenderBatch& batch )
{
    if( i == e )
    {
        return false;
    }
    UiVertex const& Vert = *i;
    Positions.push_back( Vert.Pos );
    TexCoords.push_back( Vert.TexCoord );
    Colors.push_back( Vert.Color );
    batch.TexId = Vert.TexId;
    ++i;
    return true;
}
bool IsSubtreeHidden( Widget const& wdg )
{
    for( Widget const* w = wdg.Parent(); NULL != w; w = w->Parent() )
    {
        Widget::Prop const& p = w->operator()( Widget::PT_SubtreeHidden );
        if( ( p.GetType() == Widget::Prop::T_Int || p.GetType() == Widget::Prop::T_Double )
            && p.operator int32_t() )
        {
            return true;
        }
    }
    return false;
}
}

void UiRenderer::Draw( Root const& UiRoot, const glm::mat4& projMatrix  )
{
    /*  for(Widget::const_iterator i=UiRoot.begin(),e=UiRoot.end();i!=e;++i)
        {
            static UiModelRepo const& UiModels(UiModelRepo::Get());
            Widget const& Wdg=*i;
            if(!(int32_t)Wdg(Widget::PT_Visible)) continue;
            UiModel const& Model=UiModels(Wdg.GetId());
            Model.Draw(Wdg);
        }*/

    UiVertices_t Vertices;
    Vertices.reserve( mPrevVertices.size() );
    UiVertexInserter_t Inserter( Vertices );
    for( Widget::const_iterator i = UiRoot.begin(), e = UiRoot.end(); i != e; ++i )
    {
        static UiModelRepo const& UiModels( UiModelRepo::Get() );
        Widget const& Wdg = *i;
        if( ( 0 == ( int32_t )Wdg( Widget::PT_Visible ) ) || IsSubtreeHidden( Wdg ) )
        {
            continue;
        }
        UiModel const& Model = UiModels( Wdg.GetId() );
        Model.CollectVertices( Wdg, Inserter );
    }

    size_t const CurSize = Vertices.size();
    if ( CurSize == 0 )
    {
        return;
    }

    // collect start / count values

    Positions_t Positions;
    Positions_t TexCoords;
    Positions.reserve( CurSize );
    TexCoords.reserve( CurSize );
    Colors_t Colors;
    Colors.reserve( CurSize );

    // todo: check and track changes
    UiVertices_t::const_iterator i = Vertices.begin();
    render::Counts_t const& Counts = render::count(
                                         std::bind( &getNextTextId, std::ref( i ), Vertices.end(),
                                                 std::ref( Positions ), std::ref( Colors ), std::ref( TexCoords ),
                                                 std::placeholders::_1 )
                                     );

    mVAO.Bind();
    if( CurSize != mPrevVertices.size() )
    {
        size_t TotalSize = CurSize * ( sizeof( glm::vec4 ) + 2 * sizeof( glm::vec2 ) );
        glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW );
    }

    size_t CurrentOffset = 0;
    size_t CurrentSize = CurSize * sizeof( glm::vec2 );
    GLuint CurrentAttribIndex = 0;

    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const PosIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const TexIndex = CurrentOffset;
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    size_t const ColorIndex = CurrentOffset;

    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "ui" ) );
    ShaderMgr.ActivateShader( def );
    ShaderMgr.UploadData( "time", GLfloat( platform::Clock::Now() ) );
    int w, h;
    mWindow->GetWindowSize( w, h );
    ShaderMgr.UploadData( "resolution", glm::vec2( w, h ) );
    ShaderMgr.UploadData( "uiProjection", projMatrix );
    glActiveTexture( GL_TEXTURE0 + 4 );

    for( render::Counts_t::const_iterator i = Counts.begin(), e = Counts.end(); i != e; ++i )
    {
        render::CountByTexId const& Part = *i;
        CurrentAttribIndex = 0;
        glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( PosIndex + sizeof( glm::vec2 )*Part.Start ) );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( TexIndex + sizeof( glm::vec2 )*Part.Start ) );
        ++CurrentAttribIndex;
        glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( ColorIndex + sizeof( glm::vec4 )*Part.Start ) );
        if( Part.Batch.TexId != -1 )
        {
            glBindTexture( GL_TEXTURE_2D, Part.Batch.TexId );
        }
        glDrawArrays( GL_TRIANGLES, 0, Part.Count );
    }

    mVAO.Unbind();
    // store current match
    using std::swap;
    swap( mPrevVertices, Vertices );
}

void UiRenderer::Init()
{
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "ui" ) );
    ShaderMgr.ActivateShader( def );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    ShaderMgr.UploadData( "uiTexture", GLuint( 4 ) );
    glActiveTexture( GL_TEXTURE0 );
    mWindow = engine::Engine::Get().GetSystem<engine::WindowSystem>();
}

UiRenderer::UiRenderer()
{
    Init();
}

