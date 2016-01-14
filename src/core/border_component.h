#ifndef INCLUDED_CORE_BORDER_COMPONENT_H
#define INCLUDED_CORE_BORDER_COMPONENT_H

#include "i_border_component.h"
#include "core/property_loader.h"

class BorderComponent : public IBorderComponent
{
public:
    BorderComponent();
    virtual void SetBorders(Borders_t borders);
    virtual IBorderComponent::Borders_t GetBorders()const;
    virtual void SetOuterBorders(Borders_t borders);
    virtual Borders_t GetOuterBorders()const;
    virtual void Save(Json::Value& component);
protected:
    friend class ComponentFactory;
    Borders_t mBorders;
    Borders_t mOuterBorders;
private:
};

class BorderComponentLoader : public ComponentLoader<BorderComponent>
{
    virtual void BindValues();
protected:
    BorderComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_BORDER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "border_component" -m "Borders_t-borders"
