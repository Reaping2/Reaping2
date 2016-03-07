#ifndef INCLUDED_CORE_I_ACCURACY_COMPONENT_H
#define INCLUDED_CORE_I_ACCURACY_COMPONENT_H

#include "component.h"
#include "buffable.h"
#include "platform/export.h"

class IAccuracyComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IAccuracyComponent )
    virtual Buffable<int32_t>& GetAccuracy() = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IAccuracyComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IAccuracyComponent, IAccuracyComponent, "i_accuracy_component" );
#endif//INCLUDED_CORE_I_ACCURACY_COMPONENT_H
