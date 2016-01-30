#ifndef INCLUDED_RENDER_SPRITE_H
#define INCLUDED_RENDER_SPRITE_H
#include "platform/i_platform.h"
#include "sprite_phase.h"
#include "texture.h"

class SpriteCollection;
// class Texture;

class Sprite : public Repository<SpritePhase>
{
    SpritePhase mDefaultSpritePhase;
    friend class SpriteCollection;
    GLuint mTexId;
    size_t mSteps;
    double mScale;
    double mSecsToEnd;
    Sprite( int32_t TexId, Json::Value const& Anim );
public:
    bool IsValid() const;
    double GetScale()const;
    double GetSecsToEnd()const;
    SpritePhase const& operator()( int32_t Phase ) const;
};

#endif//INCLUDED_RENDER_SPRITE_H
