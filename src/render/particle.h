#ifndef INCLUDED_RENDER_DECAL_H
#define INCLUDED_RENDER_DECAL_H
#include "platform/i_platform.h"

struct Decal
{
    glm::vec2 mCenter;
    float mHeading;
    float mRadius;
    float mAlpha;
    GLuint mTexId;
    glm::vec4 mTexCoords;
    Decal(): mHeading( 0 ), mRadius( 0 ), mAlpha( 1 ), mTexId( 0 ) {}
};

#endif//INCLUDED_RENDER_DECAL_H
