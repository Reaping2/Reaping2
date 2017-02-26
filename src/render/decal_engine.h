#ifndef INCLUDED_RENDER_DECAL_ENGINE_H
#define INCLUDED_RENDER_DECAL_ENGINE_H
#include "platform/i_platform.h"
#include "vao_base.h"
#include "particle.h"
#include "renderable_repo.h"
#include "counter.h"
#include "core/map_load_event.h"
#include "core/map_start_event.h"

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
    AutoReg mOnMapLoad;
    void OnMapLoad( core::MapLoadEvent const& Evt );
    AutoReg mOnMapStart;
    void OnMapStart( core::MapStartEvent const& Evt );
};

#endif//INCLUDED_RENDER_DECAL_ENGINE_H
