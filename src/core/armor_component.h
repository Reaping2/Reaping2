#ifndef INCLUDED_CORE_ARMOR_COMPONENT_H
#define INCLUDED_CORE_ARMOR_COMPONENT_H

#include "i_armor_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

class ArmorComponent : public IArmorComponent
{
public:
    ArmorComponent();
    virtual void SetCurrentArmor(int32_t currentArmor);
    virtual int32_t GetCurrentArmor()const;
protected:
    friend class ComponentFactory;
    int32_t mCurrentArmor;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ArmorComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IArmorComponent>(*this);
    ar & mCurrentArmor;
}

class ArmorComponentLoader : public ComponentLoader<ArmorComponent>
{
    virtual void BindValues();
protected:
    ArmorComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(ArmorComponent,"armor_component");
#endif//INCLUDED_CORE_ARMOR_COMPONENT_H


//command:  "classgenerator.exe" -g "component" -c "armor_component" -m "int32_t-currentArmor"
