#ifndef INCLUDED_CTF_FLAG_RECEIVER_COMPONENT_H
#define INCLUDED_CTF_FLAG_RECEIVER_COMPONENT_H

#include "i_flag_receiver_component.h"
#include "core/property_loader.h"

namespace ctf {

class FlagReceiverComponent : public IFlagReceiverComponent
{
public:
    FlagReceiverComponent();
protected:
    friend class ComponentFactory;
private:
};

class FlagReceiverComponentLoader : public ComponentLoader<FlagReceiverComponent>
{
    virtual void BindValues();
public:
    FlagReceiverComponentLoader();
    friend class ComponentLoaderFactory;
};

} // namespace ctf

#endif//INCLUDED_CTF_FLAG_RECEIVER_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "flag_receiver_component" -n "ctf"
