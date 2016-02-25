#ifndef INCLUDED_CORE_AUDIBLE_COMPONENT_H
#define INCLUDED_CORE_AUDIBLE_COMPONENT_H

#include "i_audible_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

class AudibleComponent : public IAudibleComponent
{
public:
    AudibleComponent();
    virtual std::vector<AudibleEffectDesc> const& GetEffects()const;
    virtual std::vector<AudibleEffectDesc>& GetEffects();
    virtual void AddOneShotEffect( int32_t id );
    virtual void AddLoopingEffect( int32_t id );
protected:
    friend class ComponentFactory;
    friend class AudibleComponentLoader;
    std::vector<AudibleEffectDesc> mEffects;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void AudibleComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IAudibleComponent>(*this);
    ar & mEffects;
}

class AudibleComponentLoader : public ComponentLoader<AudibleComponent>
{
    virtual void BindValues();
protected:
    AudibleComponentLoader();
    friend class ComponentLoaderFactory;
    std::vector<AudibleEffectDesc> mLoadDescs;
};


BOOST_CLASS_EXPORT_KEY2(AudibleComponent,"audible_component");
#endif//INCLUDED_CORE_AUDIBLE_COMPONENT_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "component" -c "audible_component" -m "int32_t-id uint32_t-uid"
