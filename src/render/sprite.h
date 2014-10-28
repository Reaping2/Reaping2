#ifndef INCLUDED_RENDER_SPRITE_H
#define INCLUDED_RENDER_SPRITE_H

class SpriteCollection;
class Texture;

class Sprite : public Repository<SpritePhase>
{
    static SpritePhase DefaultSpritePhase;
    friend class SpriteCollection;
    GLuint mTexId;
    size_t mSteps;
    double mScale;
    Sprite( int32_t TexId, Json::Value const& Anim );
public:
    bool IsValid() const;
    double GetScale()const;
    SpritePhase const& operator()( int32_t Phase ) const;
};

#endif//INCLUDED_RENDER_SPRITE_H
