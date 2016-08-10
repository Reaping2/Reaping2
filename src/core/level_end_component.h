#ifndef INCLUDED_CORE_LEVEL_END_COMPONENT_H
#define INCLUDED_CORE_LEVEL_END_COMPONENT_H

#include "i_level_end_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class LevelEndComponent : public ILevelEndComponent
{
public:
    LevelEndComponent();
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void LevelEndComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ILevelEndComponent>(*this);
}

class LevelEndComponentLoader : public ComponentLoader<LevelEndComponent>
{
    virtual void BindValues();
protected:
    LevelEndComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( LevelEndComponent, LevelEndComponent, "level_end_component" );

#endif//INCLUDED_CORE_LEVEL_END_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "level_end_component"
