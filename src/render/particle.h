#ifndef INCLUDED_RENDER_DECAL_H
#define INCLUDED_RENDER_DECAL_H

struct Decal
{
    glm::vec2 mCenter;
    int32_t mId;
    Decal(): mId( 0 ) {}
};

#endif//INCLUDED_RENDER_DECAL_H
