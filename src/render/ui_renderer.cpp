#include "i_render.h"
#include "main/window.h"
#include "engine/engine.h"

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
    struct ChangedAt
    {
        size_t Start;
        size_t Count;
    };
    for( Widget::const_iterator i = UiRoot.begin(), e = UiRoot.end(); i != e; ++i )
    {
        static UiModelRepo const& UiModels( UiModelRepo::Get() );
        Widget const& Wdg = *i;
        if( !( int32_t )Wdg( Widget::PT_Visible ) )
        {
            continue;
        }
        UiModel const& Model = UiModels( Wdg.GetId() );
        Model.CollectVertices( Wdg, Inserter );
    }

    size_t const CurSize = Vertices.size();
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions;
    Positions_t TexCoords;
    Positions.reserve( CurSize );
    TexCoords.reserve( CurSize );
    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors;
    Colors.reserve( CurSize );

    // todo: check and track changes
    for( UiVertices_t::const_iterator i = Vertices.begin(), e = Vertices.end(); i != e; ++i )
    {
        UiVertex const& Vert = *i;
        Positions.push_back( Vert.Pos );
        TexCoords.push_back( Vert.TexCoord );
        Colors.push_back( Vert.Color );
    }

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
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );

    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "ui" );
    ShaderMgr.UploadData( "time", GLfloat( glfwGetTime() ) );
    int w, h;
    mWindow->GetWindowSize( w, h );
    ShaderMgr.UploadData( "resolution", glm::vec2( w, h ) );
    ShaderMgr.UploadData( "uiProjection", projMatrix );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glDrawArrays( GL_TRIANGLES, 0, CurSize );

    mVAO.Unbind();
    // store current match
    using std::swap;
    swap( mPrevVertices, Vertices );
}

void UiRenderer::Init()
{
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "ui" );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    ShaderMgr.UploadData( "uiTexture", GLuint( 4 ) );
    glActiveTexture( GL_TEXTURE0 );
    mWindow=engine::Engine::Get().GetSystem<engine::WindowSystem>();
}

UiRenderer::UiRenderer()
{
    Init();
}

