#ifndef INCLUDED_ENGINE_SYSTEM_FACTORY_H
#define INCLUDED_ENGINE_SYSTEM_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"

#include <memory>
#include "engine/system.h"
namespace engine {
class SystemFactory : public platform::Factory<System>, public platform::Singleton<SystemFactory>
{
    friend class platform::Singleton<SystemFactory>;
    SystemFactory();
protected:
    template<typename Elem_T>
    static std::auto_ptr<System> CreateSystem( int32_t Id );
};
template<typename Elem_T>
std::auto_ptr<System> SystemFactory::CreateSystem( int32_t Id )
{
    std::auto_ptr<System> r( new Elem_T() );
    r->SetEnabled(true);
    return r;
}
} // namespace engine
#endif//INCLUDED_ENGINE_SYSTEM_FACTORY_H