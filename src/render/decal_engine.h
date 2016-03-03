#ifndef INCLUDED_RENDER_DECAL_ENGINE_H
#define INCLUDED_RENDER_DECAL_ENGINE_H
#include "platform/i_platform.h"
#include "vao_base.h"
#include "particle.h"
#include "renderable_repo.h"
#include "counter.h"

class DecalEngine : public Singleton<DecalEngine>
{
public:
    enum DecalType
    {
        GroundParticle,
        Corpse,
        NumTypes,
    };
    void Add( Decal const& Part, DecalType Typ );
    void Draw();
protected:
    VaoBase mVAO;
    friend class Singleton<DecalEngine>;
    typedef std::vector<Decal> Decals_t;
    Decals_t mDecals;
    RenderableRepo& mRenderables;
    size_t mMaxDecalsPerType;
    render::Counts_t mCounts;
    size_t mTexIndex;
    size_t mPosIndex;
    size_t mHeadingIndex;
    size_t mAlphaIndex;
    size_t mRadiusIndex;
    size_t mPrevDecalsSize;
    bool mDirty;
    DecalEngine();
    void UpdateBuffers();
};

#endif//INCLUDED_RENDER_DECAL_ENGINE_H
