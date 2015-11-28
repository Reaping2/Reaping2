#ifndef INCLUDED_RENDER_TEXTURE_H
#define INCLUDED_RENDER_TEXTURE_H
#include "platform/i_platform.h"
#include "texture_repo.h"

//class TextureRepo;
class Texture
{
    friend class TextureRepo;
    GLuint mTexId;
    GLuint mWidth;
    GLuint mHeight;
    Texture( size_t Width, size_t Height, size_t Channels, uint8_t const* Data );
public:
    ~Texture();
    GLuint TexId()const
    {
        return mTexId;
    }
    GLuint Width()const
    {
        return mWidth;
    }
    GLuint Height()const
    {
        return mHeight;
    }
};

#endif//INCLUDED_RENDER_TEXTURE_H
