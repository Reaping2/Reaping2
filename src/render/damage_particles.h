#ifndef INCLUDED_RENDER_DAMAGE_DECALS_H
#define INCLUDED_RENDER_DAMAGE_DECALS_H

#include "core/damage_taken_event.h"

class DamageDecals : public Singleton<DamageDecals>
{
    DecalEngine& mDecalEngine;
    DamageDecals();
    void Load();
    friend class Singleton<DamageDecals>;
    AutoReg mOnDamageTaken;
    std::vector<int32_t> mValidIds;
    void OnDamageTaken( core::DamageTakenEvent const& Evt );
};

#endif//INCLUDED_RENDER_DAMAGE_DECALS_H
