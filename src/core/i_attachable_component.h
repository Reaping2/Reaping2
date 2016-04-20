#ifndef INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H
#define INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H

#include "component.h"
#include "platform/export.h"
namespace ctf {

class IAttachableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IAttachableComponent )
    virtual void SetAttachedGUID( int32_t attachedGUID ) = 0;
    virtual int32_t GetAttachedGUID()const = 0;
    virtual void SetPositionX( double positionX ) = 0;
    virtual double GetPositionX()const = 0;
    virtual void SetPositionY( double positionY ) = 0;
    virtual double GetPositionY()const = 0;
    virtual void SetInheritOrientation( bool inheritOrientation ) = 0;
    virtual bool IsInheritOrientation()const = 0;
    virtual void SetRemoveOnAttachedDeath( bool removeOnAttachedDeath ) = 0;
    virtual bool IsRemoveOnAttachedDeath()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IAttachableComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}

} // namespace ctf


REAPING2_CLASS_EXPORT_KEY2( ctf__IAttachableComponent, ctf::IAttachableComponent, "i_attachable_component" );
#endif//INCLUDED_CORE_I_ATTACHABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_attachable_component" -m "int32_t-attachedGUID"
