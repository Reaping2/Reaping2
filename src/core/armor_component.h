#ifndef INCLUDED_CORE_ARMOR_COMPONENT_H
#define INCLUDED_CORE_ARMOR_COMPONENT_H

#include "i_armor_component.h"
#include "core/property_loader.h"

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
};

class ArmorComponentLoader : public ComponentLoader<ArmorComponent>
{
    virtual void BindValues();
protected:
    ArmorComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_ARMOR_COMPONENT_H


//command:  "classgenerator.exe" -g "component" -c "armor_component" -m "int32_t-currentArmor"
