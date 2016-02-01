#include "world_renderer.h"
#include "shader_manager.h"
#include <boost/assign/list_of.hpp>

namespace render {

void WorldRenderer::Init()
{
    mVAO.Init();
    GLfloat Vertices[] = {
        -1, -1,
        -1, 1,
        1, -1,
        1, 1, /* 8 */
        0, 0,
        0, 1,
        1, 0,
        1, 1,
            /* 8 */
                         };
    mVAO.Bind();
    glBufferData( GL_ARRAY_BUFFER, 16 * sizeof( GLfloat ), Vertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( void* )( 8 * sizeof( GLfloat ) ) );
    mVAO.Unbind();
}

WorldRenderer::WorldRenderer()
{
    Init();
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::Draw( double dt, GLuint texture )
{
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "world_solid_objects" );
    ShaderMgr.UploadData( "texture", GLuint( 1 ) );
    glActiveTexture( GL_TEXTURE0 + 1 );
    mVAO.Bind();
    glBindTexture( GL_TEXTURE_2D, texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    mVAO.Unbind();
    glActiveTexture( GL_TEXTURE0 );
}

}

