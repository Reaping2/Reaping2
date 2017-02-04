#ifndef INCLUDED_CORE_I_WORM_BODY_COMPONENT_H
#define INCLUDED_CORE_I_WORM_BODY_COMPONENT_H

#include "component.h"
#include "platform/i_platform.h"
#include "platform/serialize_vec4.h"

class IWormBodyComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IWormBodyComponent )
    virtual void SetHeadGUID(int32_t headGUID)=0;
    virtual int32_t GetHeadGUID()const=0;
    virtual void SetBody(bool body)=0;
    virtual bool IsBody()const=0;
    virtual void SetFollowedGUID(int32_t followedGUID)=0;
    virtual int32_t GetFollowedGUID()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IWormBodyComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}


#endif//INCLUDED_CORE_I_WORM_BODY_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "worm_body_component" -m "int32_t-headGUID bool-body int32_t-followedGUID PrevPositions_t-prevPositions"
