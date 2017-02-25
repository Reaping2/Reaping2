#ifndef INCLUDED_CORE_LIGHT_COMPONENT_H
#define INCLUDED_CORE_LIGHT_COMPONENT_H

#include "i_light_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class LightComponent : public ILightComponent
{
public:
    LightComponent();
    virtual void SetRadius( double radius );
    virtual double GetRadius() const;
    virtual void SetAperture(double radius);
    virtual double GetAperture()const;
    virtual void SetFullStrengthAperture(double radius);
    virtual double GetFullStrengthAperture()const;
protected:
    friend class ComponentFactory;
    double mRadius;
    double mAperture = 360.0;
    double mFSAperture = 360.0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void LightComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ILightComponent>(*this);
    ar& mRadius;
    ar& mAperture;
    ar& mFSAperture;
}

class LightComponentLoader : public ComponentLoader<LightComponent>
{
    virtual void BindValues();
protected:
    LightComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( LightComponent, LightComponent, "light_component" );

#endif//INCLUDED_CORE_LIGHT_COMPONENT_H

//command:  "../../build-reap-release/bin/relwithdebinfo/classgenerator" -g "component" -c "light_component" -m "double-radius"
