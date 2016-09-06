#ifndef INCLUDED_CORE_ATTACHABLE_COMPONENT_H
#define INCLUDED_CORE_ATTACHABLE_COMPONENT_H

#include "i_attachable_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

namespace ctf {

class AttachableComponent : public IAttachableComponent
{
public:
    AttachableComponent();
    virtual void SetAttachedGUID( int32_t attachedGUID );
    virtual int32_t GetAttachedGUID()const;
    virtual void SetPositionX( double positionX );
    virtual double GetPositionX()const;
    virtual void SetPositionY( double positionY );
    virtual double GetPositionY()const;
    virtual void SetInheritOrientation( bool inheritOrientation );
    virtual bool IsInheritOrientation()const;
    virtual void SetRemoveOnAttachedDeath( bool removeOnAttachedDeath );
    virtual bool IsRemoveOnAttachedDeath()const;
protected:
    friend class ComponentFactory;
    int32_t mAttachedGUID;
    double mPositionX;
    double mPositionY;
    bool mInheritOrientation;
    bool mRemoveOnAttachedDeath;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void AttachableComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IAttachableComponent>( *this );
    ar& mAttachedGUID;
    ar& mPositionX;
    ar& mPositionY;
    ar& mInheritOrientation;
    ar& mRemoveOnAttachedDeath;
}

class AttachableComponentLoader : public ComponentLoader<AttachableComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( AttachableComponentLoader )
private:
    virtual void BindValues();
public:
    AttachableComponentLoader();
    friend class ComponentLoaderFactory;
};

} // namespace ctf


REAPING2_CLASS_EXPORT_KEY2( ctf__AttachableComponent, ctf::AttachableComponent, "attachable_component" );
#endif//INCLUDED_CORE_ATTACHABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "attachable_component" -m "int32_t-attachedGUID"
