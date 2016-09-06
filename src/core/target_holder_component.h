#ifndef INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H
#define INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H

#include "i_target_holder_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class TargetHolderComponent : public ITargetHolderComponent
{
public:
    TargetHolderComponent();
    virtual void SetTargetGUID( int32_t targetId );
    virtual int32_t GetTargetGUID()const;
protected:
    friend class ComponentFactory;
    int32_t mTargetGUID;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void TargetHolderComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<ITargetHolderComponent>( *this );
    ar& mTargetGUID;
}

class TargetHolderComponentLoader : public ComponentLoader<TargetHolderComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( TargetHolderComponentLoader )
private:
    virtual void BindValues();
protected:
    TargetHolderComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( TargetHolderComponent, TargetHolderComponent, "target_holder_component" );
#endif//INCLUDED_CORE_TARGET_HOLDER_COMPONENT_H
