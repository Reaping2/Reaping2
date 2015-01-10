#ifndef INCLUDED_ENGINE_SUB_SYSTEM_FACTORY_H
#define INCLUDED_ENGINE_SUB_SYSTEM_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"

#include <memory>
#include "engine/sub_system.h"
namespace engine {
class SubSystemFactory : public platform::Factory<SubSystem>, public platform::Singleton<SubSystemFactory>
{
    friend class platform::Singleton<SubSystemFactory>;
    SubSystemFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<SubSystem> CreateSubSystem( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<SubSystem> SubSystemFactory::CreateSubSystem( int32_t Id )
{
    return std::auto_ptr<SubSystem>( new Elem_T() );
}
} // namespace engine
#endif//INCLUDED_ENGINE_SUB_SYSTEM_FACTORY_H