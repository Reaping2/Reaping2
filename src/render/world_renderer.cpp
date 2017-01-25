#include "world_renderer.h"
#include "shader_manager.h"
#include "core/opt.h"
#include "main/window.h"
#include "engine/engine.h"
#include "platform/game_clock.h"
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
    mTexIndex = 0;
    mPosIndex = 0;
    mColorIndex = 0;
}

WorldRenderer::WorldRenderer()
{
    Init();
}

WorldRenderer::~WorldRenderer()
{
}

void WorldRenderer::Draw( double dt, GLuint texture, int32_t shader, glm::vec2 const& size, GLuint secondaryTexture )
{
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( shader );
    ShaderMgr.UploadData( "texture", GLuint( 1 ) );
    ShaderMgr.UploadData( "secondaryTexture", GLuint( 2 ) );
    ShaderMgr.UploadData( "resolution", size );
    ShaderMgr.UploadData( "time", GLfloat( platform::Clock::Now() ) );
    mVAO.Bind();
    glActiveTexture( GL_TEXTURE0 + 1 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glActiveTexture( GL_TEXTURE0 + 2 );
    glBindTexture( GL_TEXTURE_2D, secondaryTexture );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glActiveTexture( GL_TEXTURE0 );
    mVAO.Unbind();
}

void WorldRenderer::Draw( double dt, GLuint texture, int32_t shader, GLuint secondaryTexture )
{
    int w, h;
    static Opt<engine::WindowSystem> window( engine::Engine::Get().GetSystem<engine::WindowSystem>() );
    window->GetWindowSize( w, h );
    Draw( dt, texture, shader, glm::vec2( w, h ), secondaryTexture );
}

}

