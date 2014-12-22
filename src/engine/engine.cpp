#include "platform/i_platform.h"
#include "engine/engine.h"
#include "collision_system.h"

namespace engine {

Engine::Engine()
    : mSystemFactory(SystemFactory::Get())
{
}

void Engine::Init()
{
    for (Systems_t::iterator it = mSystems.begin(), e = mSystems.end();it!=e;++it)
    {
        it->second->Init();
    }
}

void Engine::Update(double DeltaTime)
{
    for (Systems_t::iterator it = mSystems.begin(), e = mSystems.end();it!=e;++it)
    {
        it->second->Update( DeltaTime );
    }
}

void Engine::AddSystem(int32_t Id)
{
    mSystems.insert(Id, mSystemFactory(Id));
}

} // namespace engine
