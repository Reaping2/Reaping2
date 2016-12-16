#ifndef INCLUDED_SCRIPTEDCONTROLLER_ACT_FACTORY_H
#define INCLUDED_SCRIPTEDCONTROLLER_ACT_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "i_act.h"

namespace scriptedcontroller {

class ActFactory : public platform::Factory<IAct>, public platform::Singleton<ActFactory>
{
    friend class platform::Singleton<ActFactory>;
    ActFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<IAct> CreateIAct( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<IAct> ActFactory::CreateIAct( int32_t Id )
{
    return std::auto_ptr<IAct>( new Elem_T( Id ) );
}

} // namespace scriptedcontroller

#endif//INCLUDED_SCRIPTEDCONTROLLER_ACT_FACTORY_H

//command:  "classgenerator.exe" -g "factory" -n "scriptedcontroller" -c "act_factory" -t "i_act"
