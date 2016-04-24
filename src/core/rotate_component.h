#ifndef INCLUDED_CORE_ROTATE_COMPONENT_H
#define INCLUDED_CORE_ROTATE_COMPONENT_H

#include "i_rotate_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class RotateComponent : public IRotateComponent
{
public:
    RotateComponent();
    virtual void SetSpeed(double speed);
    virtual double GetSpeed()const;
    virtual void SetRotating(bool rotating);
    virtual bool IsRotating()const;
protected:
    friend class ComponentFactory;
    double mSpeed;
    bool mRotating;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void RotateComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IRotateComponent>(*this);
    ar& mSpeed;
    ar& mRotating;
}

class RotateComponentLoader : public ComponentLoader<RotateComponent>
{
    virtual void BindValues();
protected:
    RotateComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( RotateComponent, RotateComponent, "rotate_component" );

#endif//INCLUDED_CORE_ROTATE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "rotate_component" -m "double-speed bool-rotating"
