#include "render_target.h"
#include "platform/settings.h"
#include <numeric>

namespace render {

namespace {
GLenum internalFormatToType( GLenum fmt )
{
    static std::map<GLenum,GLenum> const typemap = 
    {
        { GL_RGBA, GL_UNSIGNED_BYTE },
        { GL_RED, GL_UNSIGNED_BYTE },
        { GL_RGBA4, GL_UNSIGNED_SHORT_4_4_4_4 },
        { GL_R3_G3_B2, GL_UNSIGNED_BYTE_3_3_2 },
    };
    auto i = typemap.find( fmt );
    return i == typemap.end() ? GL_UNSIGNED_BYTE : i->second;
}
GLenum internalFormatToFormat( GLenum fmt )
{
    static std::map<GLenum,GLenum> const typemap = 
    {
        { GL_RGBA, GL_RGBA },
        { GL_RGB, GL_RGB },
        { GL_RED, GL_RED },
        { GL_RGBA4, GL_RGBA },
        { GL_RGB4, GL_RGB },
        { GL_R3_G3_B2, GL_RGB },
    };
    auto i = typemap.find( fmt );
    return i == typemap.end() ? GL_RGBA : i->second;
}
}

RenderTargetProps::RenderTargetProps( glm::vec2 const& s,
        std::vector<GLenum> const& types )
    : Size( s )
    , AttachmentTypes( types )
{
}

RenderTarget::RenderTarget()
    : mCurrentId( ScreenId )
{
}

uint32_t RenderTarget::GetFreeId() const
{
    // temp: start from high enough, as we use hardcoded values in renderer
    // todo: use GetFreeId in renderer as well
    static uint32_t freeId = 1000;
    while( mTargets.find( ++freeId ) != mTargets.end() );
    return freeId;
}

uint32_t RenderTarget::GetCurrentTarget() const
{
    return mCurrentId;
}

void RenderTarget::SetTargetTexture( uint32_t id, RenderTargetProps const& props )
{
    TargetTexture& tgt = mTargets[ id ];
    bool recreate = tgt.Size != props.Size;
    if( tgt.TexIds.size() != props.AttachmentTypes.size() )
    {
        recreate = true;
        glBindTexture(GL_TEXTURE_2D, 0);
        tgt.TexIds.resize( props.AttachmentTypes.size() );
        tgt.Attachments.resize( props.AttachmentTypes.size() );
        tgt.AttachmentTypes = props.AttachmentTypes;

        std::iota( tgt.Attachments.begin(), tgt.Attachments.end(), GL_COLOR_ATTACHMENT0 );
        glGenFramebuffers( 1, &tgt.FramebufferId );
        glGenTextures( tgt.TexIds.size(), &tgt.TexIds[0] );
        glGenRenderbuffers(1, &tgt.DepthBufferId);
    }
    if( recreate )
    {
        tgt.Size = props.Size;

        glBindFramebuffer( GL_FRAMEBUFFER, tgt.FramebufferId );
        auto tit = tgt.TexIds.begin(), tet = tgt.TexIds.end();
        auto ait = tgt.Attachments.begin(), aet = tgt.Attachments.end();
        auto atit = tgt.AttachmentTypes.begin(), atet = tgt.AttachmentTypes.end();
        for( ; tit != tet && ait != aet && atit != atet; ++tit, ++ait, ++atit )
        {
            glBindTexture(GL_TEXTURE_2D, *tit);
            glTexImage2D(GL_TEXTURE_2D, 0, *atit, tgt.Size.x, tgt.Size.y, 0, internalFormatToFormat( *atit ), internalFormatToType( *atit ), NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, *ait, GL_TEXTURE_2D, *tit, 0);
        }

        glBindRenderbuffer(GL_RENDERBUFFER, tgt.DepthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tgt.Size.x, tgt.Size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tgt.DepthBufferId);
    }
    glBindFramebuffer( GL_FRAMEBUFFER, tgt.FramebufferId );
    glBindRenderbuffer( GL_RENDERBUFFER, tgt.DepthBufferId );
    glDrawBuffers(tgt.Attachments.size(), &tgt.Attachments[0]);
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
    glDrawBuffers(tgt.Attachments.size(), &tgt.Attachments[0]);
    bool succ = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    BOOST_ASSERT( succ );
    mCurrentId = id;
}

void RenderTarget::SetTargetScreen() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor( 0, 0, 0, 0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    mCurrentId = ScreenId;
}

GLuint RenderTarget::GetTextureId( uint32_t id, size_t attachment )
{
    auto const& ids = mTargets[ id ].TexIds;
    return attachment < ids.size() ? ids[ attachment ] : 0;
}

glm::vec2 RenderTarget::GetMaxTextureSize() const
{
    int s;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &s );
    static Settings& settings( Settings::Get() );
    // some nvidia cards report 16k supported size, but fail when requesting a texture with that size
    static int maxAllowedTextureSize = settings.GetInt( "graphics.gl.max_allowed_texture_size", 8196 );
    s = std::min( s, maxAllowedTextureSize );
    return glm::vec2( s, s );
}

}

