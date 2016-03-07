#include "i_render.h"
#include "vao_base.h"

namespace {
GLint maxTextureUnits()
{
    static GLint tu = -1;
    if( tu == -1 )
    {
        glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &tu );
    }
    return tu;
}
}

void VaoBase::Init()
{
    glGenBuffers( 1, &mDataBuffer );

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glBindBuffer( GL_ARRAY_BUFFER, mDataBuffer );

    glGenSamplers( 1, &mSampler );
    glSamplerParameteri( mSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glSamplerParameteri( mSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glSamplerParameteri( mSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glSamplerParameteri( mSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glSamplerParameteri( mSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    Unbind();
}

void VaoBase::Bind()const
{
    glBindVertexArray( mVAO );
    glBindBuffer( GL_ARRAY_BUFFER, mDataBuffer );
    for( GLint i = 0, e = maxTextureUnits(); i != e; ++ i )
    {
        glBindSampler( i, mSampler );
    }
}

void VaoBase::Unbind()const
{
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    for( GLint i = 0, e = maxTextureUnits(); i != e; ++ i )
    {
        glBindSampler( i, 0 );
    }
}

VaoBase::VaoBase()
    : mVAO( 0xffffffff )
    , mDataBuffer( 0xffffffff )
    , mSampler( 0xffffffff )
{

}

VaoBase::~VaoBase()
{
    Unbind();
    if( mSampler != 0xffffffff )
    {
        glDeleteSamplers( 1, &mSampler );
    }
    if( mDataBuffer != 0xffffffff )
    {
        glDeleteBuffers( 1, &mDataBuffer );
    }
    if( mVAO != 0xffffffff )
    {
        glDeleteVertexArrays( 1, &mVAO );
    }
}
