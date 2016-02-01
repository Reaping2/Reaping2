#include "world_renderer.h"
#include "shader_manager.h"
#include <boost/assign/list_of.hpp>

namespace render {

void WorldRenderer::Init()
{
    mVAO.Init();
    typedef std::vector<glm::vec2> Positions_t;
    Positions_t Positions = boost::assign::list_of
        ( glm::vec2( -1000, -1000 ) )
        ( glm::vec2( -1000,  1000 ) )
        ( glm::vec2(  1000,  1000 ) )
        ( glm::vec2(  1000, -1000 ) );
    typedef std::vector<glm::vec2> TexCoords_t;
    TexCoords_t TexCoords = boost::assign::list_of
        ( glm::vec2( 0, 0 ) )
        ( glm::vec2( 0, 1 ) )
        ( glm::vec2( 1, 1 ) )
        ( glm::vec2( 1, 0 ) );
    typedef std::vector<glm::vec4> Colors_t;
    Colors_t Colors = boost::assign::list_of
        ( glm::vec4( 1, 1, 1, 1 ) )
        ( glm::vec4( 1, 1, 1, 1 ) )
        ( glm::vec4( 1, 1, 1, 1 ) )
        ( glm::vec4( 1, 1, 1, 1 ) );
    mVAO.Bind();
    size_t TotalSize = Positions.size() * ( sizeof( glm::vec2 ) * 2 + sizeof( glm::vec4 ) );
    glBufferData( GL_ARRAY_BUFFER, TotalSize, NULL, GL_STATIC_DRAW );

    size_t CurrentOffset = 0;
    size_t CurrentSize = Positions.size() * sizeof( glm::vec2 );
    GLuint CurrentAttribIndex = 0;
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &TexCoords[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mTexIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = Positions.size() * sizeof( glm::vec2 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Positions[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mPosIndex = CurrentOffset;
    ++CurrentAttribIndex;

    CurrentOffset += CurrentSize;
    CurrentSize = Positions.size() * sizeof( glm::vec4 );
    glBufferSubData( GL_ARRAY_BUFFER, CurrentOffset, CurrentSize, &Colors[0] );
    glEnableVertexAttribArray( CurrentAttribIndex );
    mColorIndex = CurrentOffset;
    mVAO.Unbind();
}

WorldRenderer::WorldRenderer()
{
    Init();
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::Draw( double dt, GLuint solidObjects, GLuint particles )
{
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "world_solid_objects" );
    ShaderMgr.UploadData( "texture", GLuint( 1 ) );
    glActiveTexture( GL_TEXTURE0 + 1 );
    mVAO.Bind();
    size_t CurrentAttribIndex = 0;
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mTexIndex ));
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mPosIndex ));
    ++CurrentAttribIndex;
    glVertexAttribPointer( CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, ( GLvoid* )( mColorIndex ));
    glBindTexture( GL_TEXTURE_2D, solidObjects );
    glDrawArrays( GL_QUADS, 0, 1 );
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

}

