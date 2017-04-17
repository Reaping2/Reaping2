#ifndef INCLUDED_CORE_ACTIVATABLE_COMPONENT_H
#define INCLUDED_CORE_ACTIVATABLE_COMPONENT_H

#include "i_activatable_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ActivatableComponent : public IActivatableComponent
{
public:
    ActivatableComponent();
    virtual void SetEnabled( bool enabled );
    virtual bool IsEnabled() const;
    virtual void SetActivatorGUID( int32_t guid );
    virtual int32_t GetActivatorGUID() const;
    virtual bool IsActivated() const;
    virtual void SetHighlighted( bool highlighted );
    virtual bool IsHighlighted() const;
protected:
    friend class ComponentFactory;
    bool mEnabled;
    int32_t mActivatorGUID;
    bool mHighlighted;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ActivatableComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IActivatableComponent>(*this);
    ar& mEnabled;
    ar& mActivatorGUID;
    ar& mHighlighted;
}

class ActivatableComponentLoader : public ComponentLoader<ActivatableComponent>
{
    virtual void BindValues();
protected:
    ActivatableComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( ActivatableComponent, ActivatableComponent, "activatable_component" );

#endif//INCLUDED_CORE_ACTIVATABLE_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "activatable_component" -m "bool-enabled bool-activated bool-highlighted"
