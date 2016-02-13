#ifndef INCLUDED_CORE_CLOAK_COMPONENT_H
#define INCLUDED_CORE_CLOAK_COMPONENT_H

#include "i_cloak_component.h"
#include "core/property_loader.h"

class CloakComponent : public ICloakComponent
{
public:
    CloakComponent();
    virtual void SetActive(bool active);
    virtual bool IsActive()const;
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
protected:
    friend class ComponentFactory;
    bool mActive;
private:
};

template<class Archive>
void CloakComponent::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Component>(*this);
    ar & mActive;
}

class CloakComponentLoader : public ComponentLoader<CloakComponent>
{
    virtual void BindValues();
protected:
    CloakComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_CLOAK_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "cloak_component" -m "bool-active"
