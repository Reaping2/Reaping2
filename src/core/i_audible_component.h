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
    AudibleEffectDesc();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void AudibleEffectDesc::serialize(Archive& ar, const unsigned int version)
{
    ar & UID;
    ar & Id;
    ar & TTL;
    ar & AutoLoopUntilDeath;
}

class IAudibleComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IAudibleComponent)
    virtual std::vector<AudibleEffectDesc> const& GetEffects()const=0;
    virtual std::vector<AudibleEffectDesc>& GetEffects()=0;
    virtual void AddOneShotEffect( int32_t id )=0;
    virtual void AddLoopingEffect( int32_t id )=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void IAudibleComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_AUDIBLE_COMPONENT_H

//command:  "classgenerator" -g "i_component" -c "i_audible_component" -m "std::vector<AudibleEffectDesc>-effects int32_t-uid"
