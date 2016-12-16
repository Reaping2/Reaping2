#ifndef INCLUDED_SCRIPTEDCONTROLLER_CONDITION_FACTORY_H
#define INCLUDED_SCRIPTEDCONTROLLER_CONDITION_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "i_condition.h"

namespace scriptedcontroller {

class ConditionFactory : public platform::Factory<ICondition>, public platform::Singleton<ConditionFactory>
{
    friend class platform::Singleton<ConditionFactory>;
    ConditionFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<ICondition> CreateICondition( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<ICondition> ConditionFactory::CreateICondition( int32_t Id )
{
    return std::auto_ptr<ICondition>( new Elem_T( Id ) );
}

} // namespace scriptedcontroller

#endif//INCLUDED_SCRIPTEDCONTROLLER_CONDITION_FACTORY_H

//command:  "classgenerator.exe" -g "factory" -n "scriptedcontroller" -c "condition_factory" -t "i_condition"
