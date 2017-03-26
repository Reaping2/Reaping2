#ifndef INCLUDED_CORE_ATTRACTOR_COMPONENT_H
#define INCLUDED_CORE_ATTRACTOR_COMPONENT_H

#include "i_attractor_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <boost/serialization/vector.hpp>

class AttractorComponent : public IAttractorComponent
{
public:
    AttractorComponent();
    virtual void SetAttractorDescs( AttractorDescs_t attractorDescs );
    virtual AttractorDescs_t& GetAttractorDescs();
protected:
    friend class ComponentFactory;
    AttractorDescs_t mAttractorDescs;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void AttractorComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IAttractorComponent>(*this);
    ar& mAttractorDescs;
}

class AttractorComponentLoader : public ComponentLoader<AttractorComponent>
{
    virtual void BindValues();
protected:
    AttractorComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( AttractorComponent, AttractorComponent, "attractor_component" );

#endif//INCLUDED_CORE_ATTRACTOR_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attractor_component" -m "AttractorDescs_t-attractorDescs"
