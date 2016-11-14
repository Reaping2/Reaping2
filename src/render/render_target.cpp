#include "render_target.h"

namespace render {

RenderTarget::RenderTarget()
    : mCurrentId( ScreenId )
{
}

uint32_t RenderTarget::GetFreeId() const
{
    // temp: start from high enough, as we use hardcoded values in renderer
    // todo: use GetFreeId in renderer as well
    uint32_t freeId = 1000;
    while( mTargets.find( freeId ) != mTargets.end() )
    {
        ++freeId;
    }
    return freeId;
}

uint32_t RenderTarget::GetCurrentTarget() const
{
    return mCurrentId;
}

void RenderTarget::SetTargetTexture( uint32_t id, glm::vec2 const& size )
{
    TargetTexture& tgt = mTargets[ id ];
    if( tgt.TexId == 0 )
    {
        glBindTexture(GL_TEXTURE_2D, 0);

        static GLenum attach = GL_COLOR_ATTACHMENT0;
        tgt.Attachment = attach++;
        glGenFramebuffers( 1, &tgt.FramebufferId );
        glGenTextures( 1, &tgt.TexId );
        glGenRenderbuffers(1, &tgt.DepthBufferId);
    }
    if( tgt.Size != size )
    {
        tgt.Size = size;

        glBindFramebuffer( GL_FRAMEBUFFER, tgt.FramebufferId );
        glBindTexture(GL_TEXTURE_2D, tgt.TexId);

        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, tgt.Size.x, tgt.Size.y, 0,GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, tgt.Attachment, GL_TEXTURE_2D, tgt.TexId, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, tgt.DepthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tgt.Size.x, tgt.Size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tgt.DepthBufferId);
    }
    glBindFramebuffer( GL_FRAMEBUFFER, tgt.FramebufferId );
    glBindRenderbuffer( GL_RENDERBUFFER, tgt.DepthBufferId );
    GLenum drawBuffers[1] = { tgt.Attachment };
    glDrawBuffers(1, drawBuffers);
    bool succ = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    BOOST_ASSERT( succ );
    glClearColor( 0, 0, 0, 0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mCurrentId = id;
}

void RenderTarget::SelectTargetTexture( uint32_t id ) const
{
    if( id == ScreenId )
    {
        SetTargetScreen();
        return;
    }
    TargetTexture const& tgt = mTargets.at( id );
    glBindFramebuffer( GL_FRAMEBUFFER, tgt.FramebufferId );
    glBindRenderbuffer( GL_RENDERBUFFER, tgt.DepthBufferId );
    GLenum drawBuffers[1] = { tgt.Attachment };
    glDrawBuffers(1, drawBuffers);
    bool succ = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    BOOST_ASSERT( succ );
    mCurrentId = id;
}

void RenderTarget::SetTargetScreen() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor( 0, 0, 0, 0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mCurrentId = ScreenId;
}

GLuint RenderTarget::GetTextureId( uint32_t id )
{
    return mTargets[ id ].TexId;
}

glm::vec2 RenderTarget::GetMaxTextureSize() const
{
    int s;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &s );
    return glm::vec2( s, s );
}

}

