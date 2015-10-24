#ifndef INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H
#define INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H

#include "i_flag_carrier_component.h"
#include "core/property_loader.h"

namespace ctf {

class FlagCarrierComponent : public IFlagCarrierComponent
{
public:
    FlagCarrierComponent();
protected:
    friend class ComponentFactory;
private:
};

class FlagCarrierComponentLoader : public ComponentLoader<FlagCarrierComponent>
{
    virtual void BindValues();
protected:
    FlagCarrierComponentLoader();
    friend class ComponentLoaderFactory;
};

} // namespace ctf

#endif//INCLUDED_CTF_FLAG_CARRIER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "flag_carrier_component" -n "ctf"
