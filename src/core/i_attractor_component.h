#ifndef INCLUDED_CORE_I_ATTRACTOR_COMPONENT_H
#define INCLUDED_CORE_I_ATTRACTOR_COMPONENT_H

#include "component.h"
#include "collision_class.h"
#include "json/json.h"
#include <boost/serialization/set.hpp>

class IAttractorComponent : public Component
{
public:
    struct AttractorDesc
    {
        using AttractTypes_t = std::set<int32_t>;
        int32_t mCollisionClassMask;
        AttractTypes_t mAttractTypes;
        double mRange = 1000.0;
        double mAcceleration = 500.0;
        template<class Archive>
        void serialize( Archive& ar, const unsigned int version );
        virtual void Load( Json::Value const& setters );
    };

    using AttractorDescs_t = std::vector<AttractorDesc>;
    DEFINE_COMPONENT_BASE(IAttractorComponent)
    virtual void SetAttractorDescs(AttractorDescs_t attractorDescs)=0;
    virtual AttractorDescs_t& GetAttractorDescs()=0;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IAttractorComponent::AttractorDesc::serialize( Archive& ar, const unsigned int version )
{
    ar& mCollisionClassMask;
    ar& mAttractTypes;
    ar& mRange;
    ar& mAcceleration;
}

template<class Archive>
void IAttractorComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_ATTRACTOR_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attractor_component" -m "AttractorDescs_t-attractorDescs"
