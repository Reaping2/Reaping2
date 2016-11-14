#pragma once
#ifndef INCLUDED_SPRITE_PHASE_CACHE_H
#define INCLUDED_SPRITE_PHASE_CACHE_H

#include "sprite_phase.h"
#include "vao_base.h"

#include "platform/singleton.h"
#include "platform/glm_fix.h"

#include <map>
#include <set>

namespace render {

class SpritePhaseCache : public platform::Singleton<SpritePhaseCache>
{
    std::map<SpritePhase const*, SpritePhase> mOriginal;
    struct ExtData
    {
        glm::vec4 freeRegion;
        glm::vec4 scaledRegion;
        float size;
    };
    std::map<SpritePhase const*,ExtData> mPending;
    uint32_t mTarget;
    glm::vec2 mTargetSize;
    int mMaxCellSize;
    int mCacheIndex = 0;
    int mRowSize = 0;
    VaoBase mVAO;
    friend class platform::Singleton<SpritePhaseCache>;
    SpritePhaseCache();
    glm::vec4 FindFreeRegion( SpritePhase const& sprphase );
    void Draw( SpritePhase const& sprphase, glm::vec4 const& freeRegion, glm::vec4 const& scaledRegion );
public:
    GLuint mTargetTexId;
    void ProcessPending();
    void Request( SpritePhase const& sprphase, float size );
};

}

#endif // INCLUDED_SPRITE_PHASE_CACHE_H

