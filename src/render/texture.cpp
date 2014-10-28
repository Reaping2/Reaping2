#include "i_render.h"

Texture::Texture( size_t Width, size_t Height, size_t Channels, uint8_t const* Data )
    : mWidth( Width )
    , mHeight( Height )
    , mTexId( 0 )
{
    if( Channels != 3 && Channels != 4 )
    {
        return;
    }
    // itt lehetne power of two-va alakitani a texturet
    // de nincs ra szukseg opengl2.0 ota
    glGenTextures( 1, &mTexId );
    glBindTexture( GL_TEXTURE_2D, mTexId );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0,
                  Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ( GLvoid* ) Data );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

Texture::~Texture()
{
    if( mTexId )
    {
        glDeleteTextures( 1, &mTexId );
    }
}

