#ifndef INCLUDED_ENGINE_ENGINE_H
#define INCLUDED_ENGINE_ENGINE_H

#include "engine/system.h"

#include <boost/ptr_container/ptr_map.hpp>
#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "system_factory.h"

namespace engine {

class Engine : public platform::Singleton<Engine>
{
    friend class platform::Singleton<Engine>;
    typedef boost::ptr_map<int32_t, System> Systems_t;
    Systems_t mSystems;
    SystemFactory& mSystemFactory;
public:
    Engine();
    void Init();
    void Update( double DeltaTime );
    void AddSystem( int32_t Id );
};

} // namespace engine

#endif//INCLUDED_ENGINE_COLLISION_SYSTEM_H