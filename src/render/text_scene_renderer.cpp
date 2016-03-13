#include "i_render.h"
#include "text_scene_renderer.h"
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

void TextSceneRenderer::Init()
{
    mVAO.Init();
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "text_scene" );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    ShaderMgr.UploadData( "uiTexture", GLuint( 4 ) );
    glActiveTexture( GL_TEXTURE0 );
    mWindow = engine::Engine::Get().GetSystem<engine::WindowSystem>();
}

TextSceneRenderer::TextSceneRenderer()
{
    Init();
}

void TextSceneRenderer::Draw()
{
    UiVertices_t Vertices;
    Vertices.reserve( mPrevVertices.size() );
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions;
    Positions_t TexCoords;
    Positions.reserve( mPrevVertices.size() );
    TexCoords.reserve( mPrevVertices.size() );
    Positions_t TextPosition;
    TextPosition.reserve( mPrevVertices.size() );

    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors;
    Colors.reserve( mPrevVertices.size() );
    UiVertexInserter_t Inserter( Vertices );
    struct ChangedAt
    {
        size_t Start;
        size_t Count;
    };
    int32_t lastVertexIndex = 0;

    for( Texts_t::const_iterator i = mTexts.begin(), e = mTexts.end(); i != e; ++i )
    {
        Text const& text = *i;
        TextUiModel::CollectVertices( text, Inserter );
        if ( Vertices.empty() )
        {
            continue;
        }
        UiVertex vertex = Vertices.back();
        float correction = text.mAlignMiddle ? ( vertex.Pos.x + vertex.TexCoord.x ) / 2.0f : 0.0f;

        // todo: check and track changes
        for( size_t i = lastVertexIndex, e = Vertices.size(); i != e; ++i )
        {
            UiVertex const& Vert = Vertices[i];
            Positions.push_back( Vert.Pos );
            TexCoords.push_back( Vert.TexCoord );
            Colors.push_back( Vert.Color );
            TextPosition.push_back( glm::vec2( text.mPosition.x - correction, text.mPosition.y ) );
        }
        lastVertexIndex = Vertices.size();
        lastVertexIndex = std::max( 0, lastVertexIndex );
    }
    size_t const CurSize = Vertices.size();
    if ( CurSize == 0 )
    {
        return;
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
    ++CurrentAttribIndex;
    CurrentOffset += CurrentSize;
    CurrentSize = CurSize * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TextPosition[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )CurrentOffset );




    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "text_scene" );
    int w, h;
    mWindow->GetWindowSize( w, h );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    glDrawArrays( GL_TRIANGLES, 0, CurSize );

    mVAO.Unbind();
    // store current match
    using std::swap;
    swap( mPrevVertices, Vertices );
    mTexts.clear();
}

void TextSceneRenderer::AddText( Text const& text )
{
    mTexts.push_back( text );
}

