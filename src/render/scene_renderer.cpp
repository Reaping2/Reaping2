#include "i_render.h"

SceneRenderer::SceneRenderer()
    : mRenderables( RenderableRepo::Get() )
    , mSceneTypeId( -1 )
    , mTextureId( 0xffffffff )
{
    mVAO.Init();
    mActionId = AutoId( "default_action" );
}

void SceneRenderer::Draw( Scene& Object )
{
    if( SceneChanged( Object ) )
    {
        CreateMesh( Object );
    }


    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "scene" );
    mVAO.Bind();
    glActiveTexture( GL_TEXTURE0 + 2 );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    mVAO.Unbind();
    glActiveTexture( GL_TEXTURE0 );
}

void SceneRenderer::CreateMesh( Scene& Object )
{
    mSceneDimensions = Object.GetDimensions();
    mSceneTypeId = Object.GetTypeId();
    Sprite const& Spr = mRenderables( mSceneTypeId )( mActionId );
    if( !Spr.IsValid() )
    {
        return;
    }
    SpritePhase const& Phase = Spr( 0 );

    GLfloat Vertices[] = {mSceneDimensions.x, mSceneDimensions.y, mSceneDimensions.x, mSceneDimensions.w, mSceneDimensions.z, mSceneDimensions.y, mSceneDimensions.z, mSceneDimensions.w, /*8*/
                          Phase.Left, Phase.Bottom, Phase.Right, Phase.Bottom, Phase.Left, Phase.Top, Phase.Right, Phase.Top/*8*/
                         };
    mVAO.Bind();
    glBufferData( GL_ARRAY_BUFFER, 16 * sizeof( GLfloat ), Vertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, ( void* )( 8 * sizeof( GLfloat ) ) );
    mVAO.Unbind();
    mTextureId = Phase.TexId;
    ShaderManager& ShaderMgr( ShaderManager::Get() );
    ShaderMgr.ActivateShader( "scene" );
    glActiveTexture( GL_TEXTURE0 + 2 );
    glBindTexture( GL_TEXTURE_2D, mTextureId );
    ShaderMgr.UploadData( "sceneTexture", GLuint( 2 ) );
}

bool SceneRenderer::SceneChanged( Scene& Object ) const
{
    return mSceneDimensions != Object.GetDimensions() || mSceneTypeId != Object.GetTypeId();
}
