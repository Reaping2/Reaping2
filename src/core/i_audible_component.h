#ifndef INCLUDED_CORE_I_AUDIBLE_COMPONENT_H
#define INCLUDED_CORE_I_AUDIBLE_COMPONENT_H

#include "component.h"

struct AudibleEffectDesc {
    static const int32_t TTL_Infinity;
    int32_t UID;
    int32_t Id;
    int32_t TTL;
    bool AutoLoopUntilDeath;
    AudibleEffectDesc( int32_t id, bool autoLoopUntilDeath=false );
};

class IAudibleComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAudibleComponent)
    virtual std::vector<AudibleEffectDesc> const& GetEffects()const=0;
    virtual std::vector<AudibleEffectDesc>& GetEffects()=0;
    virtual void AddOneShotEffect( int32_t id )=0;
    virtual void AddLoopingEffect( int32_t id )=0;
};

#endif//INCLUDED_CORE_I_AUDIBLE_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_audible_component" -m "std::vector<AudibleEffectDesc>-effects int32_t-uid"
