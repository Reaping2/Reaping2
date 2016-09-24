#ifndef INCLUDED_CORE_POINTER_TARGET_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_POINTER_TARGET_CONTROLLER_COMPONENT_H

#include "controller_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class PointerTargetControllerComponent : public ControllerComponent
{
public:
    PointerTargetControllerComponent();
    virtual void SetPointedTargetGUID( int32_t pointedTargetId );
    virtual int32_t GetPointedTargetGUID()const;
    virtual void SetPointedTargetCounter( int32_t pointedTargetCounter );
    virtual int32_t GetPointedTargetCounter()const;
    virtual void SetNextLaserCounter( double nextLaserCounter );
    virtual double GetNextLaserCounter()const;
    virtual void SetNextLaserFrequency( double nextLaserFrequency );
    virtual double GetNextLaserFrequency()const;
    virtual void SetHeadingModifierCounter( double headingModifierCounter );
    virtual double GetHeadingModifierCounter()const;
    virtual void SetHeadingModifierFrequency( double headingModifierFrequency );
    virtual double GetHeadingModifierFrequency()const;
protected:
    friend class ComponentFactory;
    int32_t mPointedTargetGUID;
    int32_t mPointedTargetCounter;
    double mNextLaserCounter;
    double mNextLaserFrequency;
    double mHeadingModifierCounter;
    double mHeadingModifierFrequency;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void PointerTargetControllerComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ControllerComponent>( *this );
    ar& mPointedTargetGUID;
    ar& mPointedTargetCounter;
    ar& mNextLaserCounter;
    ar& mNextLaserFrequency;
    ar& mHeadingModifierCounter;
    ar& mHeadingModifierFrequency;
}

class PointerTargetControllerComponentLoader : public ComponentLoader<PointerTargetControllerComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( PointerTargetControllerComponentLoader )
private:
    virtual void BindValues();
protected:
    PointerTargetControllerComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( PointerTargetControllerComponent, PointerTargetControllerComponent, "pointer_target_controller_component" );
#endif//INCLUDED_CORE_POINTER_TARGET_CONTROLLER_COMPONENT_H

