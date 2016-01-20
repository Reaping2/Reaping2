#ifndef INCLUDED_CORE_I_AUDIBLE_COMPONENT_H
#define INCLUDED_CORE_I_AUDIBLE_COMPONENT_H

#include "component.h"

struct AudibleEffectDesc {
    int32_t UID;
    int32_t Id;
    AudibleEffectDesc( int32_t uid, int32_t id )
        : UID( uid ), Id( id ) {}
};

class IAudibleComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAudibleComponent)
    virtual std::vector<AudibleEffectDesc> const& GetEffects()const=0;
    virtual std::vector<AudibleEffectDesc>& GetEffects()=0;
};

#endif//INCLUDED_CORE_I_AUDIBLE_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_audible_component" -m "std::vector<AudibleEffectDesc>-effects int32_t-uid"
