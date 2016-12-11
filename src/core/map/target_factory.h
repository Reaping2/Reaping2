#ifndef INCLUDED_MAP_TARGET_FACTORY_H
#define INCLUDED_MAP_TARGET_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "i_target.h"

namespace map {

class TargetFactory : public platform::Factory<ITarget>, public platform::Singleton<TargetFactory>
{
    friend class platform::Singleton<TargetFactory>;
    TargetFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<ITarget> CreateITarget( int32_t Id );
};

template<typename Elem_T>
std::auto_ptr<ITarget> TargetFactory::CreateITarget( int32_t Id )
{
    return std::auto_ptr<ITarget>( new Elem_T() );
}

} // namespace map

#endif//INCLUDED_MAP_TARGET_FACTORY_H

//command:  "./classgenerator" -g "factory" -c "target_factory" -t "i_target" -n "map"
