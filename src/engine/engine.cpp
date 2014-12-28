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
    
    for (Systems_t::const_iterator it = mSystemHolder.mSystems.begin(), e = mSystemHolder.mSystems.end();it!=e;++it)
    {
        it->mSystem->Init();
    }
}

void Engine::Update(double DeltaTime)
{
    SystemsFilter<Engine::EnabledSystems> enabledSystems(mSystemHolder.mSystems);
    for (SystemsFilter<Engine::EnabledSystems>::const_iterator it = enabledSystems.begin(), e = enabledSystems.end();it!=e;++it)
    {
        it->mSystem->Update( DeltaTime );
    }
}

void Engine::AddSystem(int32_t Id)
{
    mSystemHolder.mSystems.insert(SystemElement(Id,mSystemHolder.mSystems.size(),true, Opt<System>(mSystemFactory(Id).release())));
}

Engine::~Engine()
{
    for (Systems_t::const_iterator it = mSystemHolder.mSystems.begin(), e = mSystemHolder.mSystems.end();it!=e;++it)
    {
        delete &*it->mSystem;
    }
    mSystemHolder.mSystems.clear();
}

void Engine::SetEnabled(int32_t Id, bool enabled)
{
    Systems_t::iterator it = mSystemHolder.mSystems.find(Id);
    mSystemHolder.mSystems.modify(it,SystemEnableModifier(enabled));
}

Opt<System> Engine::GetSystem(int32_t Id)
{
    Systems_t::iterator it = mSystemHolder.mSystems.find(Id);
    return it!=mSystemHolder.mSystems.end()?it->mSystem:Opt<System>(NULL);
}







int32_t SystemHolder::SystemDefaultOrderer::operator()(const SystemElement& system) const
{
    return system.mOrder;
}


int32_t SystemHolder::SystemIdOrderer::operator()(const SystemElement& system) const
{
    return system.mId;
}


bool SystemHolder::IsEnabled::operator()(const SystemElement& system) const
{
    return system.mEnabled;
}


SystemElement::SystemElement(int32_t id, int32_t order, bool enabled, Opt<System> system)
    : mId(id)
    , mOrder(order)
    , mEnabled(enabled)
    , mSystem(system)
{

}


SystemEnableModifier::SystemEnableModifier(bool enabled)
    : mEnabled(enabled)
{

}

void SystemEnableModifier::operator()(SystemElement& system)
{
    system.mEnabled=mEnabled;
}

} // namespace engine
