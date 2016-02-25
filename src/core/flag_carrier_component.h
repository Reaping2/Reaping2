#ifndef INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H
#define INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H

#include "i_flag_carrier_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

namespace ctf {

class FlagCarrierComponent : public IFlagCarrierComponent
{
public:
    FlagCarrierComponent();
protected:
    friend class ComponentFactory;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void FlagCarrierComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IFlagCarrierComponent>(*this);
}

class FlagCarrierComponentLoader : public ComponentLoader<FlagCarrierComponent>
{
    virtual void BindValues();
public:
    FlagCarrierComponentLoader();
    friend class ComponentLoaderFactory;
};

} // namespace ctf


BOOST_CLASS_EXPORT_KEY2(ctf::FlagCarrierComponent,"flag_carrier_component");
#endif//INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "flag_carrier_component" -n "ctf"
