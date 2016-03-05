#ifndef INCLUDED_CORE_ACCURACY_COMPONENT_H
#define INCLUDED_CORE_ACCURACY_COMPONENT_H

#include "i_accuracy_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class AccuracyComponent : public IAccuracyComponent
{
public:
    AccuracyComponent();
    void SetAccuracyBase(int32_t accuracy);
    virtual Buffable<int32_t>& GetAccuracy();
protected:
    friend class ComponentFactory;
    Buffable<int32_t> mAccuracy;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void AccuracyComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IAccuracyComponent>(*this);
    ar & mAccuracy;
}

class AccuracyComponentLoader : public ComponentLoader<AccuracyComponent>
{
    virtual void BindValues();
protected:
    AccuracyComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2(AccuracyComponent, AccuracyComponent,"accuracy_component");
#endif//INCLUDED_CORE_ACCURACY_COMPONENT_H

