#ifndef INCLUDED_RENDER_DECAL_ENGINE_H
#define INCLUDED_RENDER_DECAL_ENGINE_H
#include "platform/i_platform.h"
#include "vao_base.h"
#include "particle.h"
#include "renderable_repo.h"

class DecalEngine : public Singleton<DecalEngine>
{
public:
    enum DecalType
    {
        GroundParticle,
        NumTypes,
    };
    void Add( Decal const& Part, DecalType Typ );
    void Draw( DecalType Type );
protected:
    VaoBase mVAO;
    // all decals MUST be on the same texture
    // this way we don't have to track them on client side, only on gpu
    // well, multiple samplers would make it possible to use multiple texids, but really, it's cleaner this way
    GLuint mTexId;
    friend class Singleton<DecalEngine>;
    typedef std::vector<Decal> Particles_t;
    RenderableRepo& mRenderables;
    Particles_t mNewDecals[NumTypes];
    size_t mNumDecals[NumTypes];
    size_t mNextIdx[NumTypes];
    size_t mMaxDecalsPerType;
    size_t mOneTypeSize;
    DecalEngine();
    void UpdateBuffers( DecalType Typ );
};

#endif//INCLUDED_RENDER_DECAL_ENGINE_H
