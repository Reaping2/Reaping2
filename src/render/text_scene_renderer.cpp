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
    static int32_t def( AutoId( "text_scene" ) );
    ShaderMgr.ActivateShader( def );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    ShaderMgr.UploadData( "uiTexture", GLuint( 4 ) );
    glActiveTexture( GL_TEXTURE0 );
}

TextSceneRenderer::TextSceneRenderer()
    : mPrevVerticesSize( 0 )
{
    Init();
}

void TextSceneRenderer::Draw()
{
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions;
    Positions_t TexCoords;
    Positions.reserve( mPrevVerticesSize );
    TexCoords.reserve( mPrevVerticesSize );
    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors;
    Colors.reserve( mPrevVerticesSize );

    size_t CurSize = 0;
    for( Texts_t::const_iterator i = mTexts.begin(), e = mTexts.end(); i != e; ++i )
    {
        UiVertices_t Vertices;
        UiVertexInserter_t Inserter( Vertices );
        Text const& text = *i;
        TextUiModel::CollectVertices( text, Inserter, false );
        for( size_t i = 0; i + 5 < Vertices.size(); i += 6 )
        {   // for every 6 vertex: swap top-bottom texcoord
            using std::swap;
            swap( Vertices[i].TexCoord.y, Vertices[i+1].TexCoord.y );
            swap( Vertices[i+2].TexCoord.y, Vertices[i+4].TexCoord.y );
            swap( Vertices[i+3].TexCoord.y, Vertices[i+5].TexCoord.y );
        }
        for( auto& Vert : Vertices )
        {
            Vert.Pos.x -= text.mDimensions.z / 2.0;
            Positions.push_back( Vert.Pos );
            TexCoords.push_back( Vert.TexCoord );
            Colors.push_back( Vert.Color );
        }
        CurSize += Vertices.size();
    }
    if ( CurSize == 0 )
    {
        return;
    }
    mVAO.Bind();
    if( CurSize != mPrevVerticesSize )
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

    ShaderManager& ShaderMgr( ShaderManager::Get() );
    static int32_t def( AutoId( "text_scene" ) );
    ShaderMgr.ActivateShader( def );
    glActiveTexture( GL_TEXTURE0 + 4 );
    glBindTexture( GL_TEXTURE_2D, Font::Get().GetTexId() );
    glDrawArrays( GL_TRIANGLES, 0, CurSize );

    mVAO.Unbind();
    mPrevVerticesSize = CurSize;
    mTexts.clear();
}

void TextSceneRenderer::AddText( Text const& text )
{
    mTexts.push_back( text );
}

