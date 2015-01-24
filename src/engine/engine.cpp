#include "platform/i_platform.h"
#include "engine/engine.h"

namespace engine {

Engine::Engine()
    : mSystemFactory(SystemFactory::Get())
    , mIsRunning(false)
{
}
void Engine::OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
    if( Evt.CurrentPhase == ProgramPhase::InitiateShutdown )
    {
        mIsRunning=false;
    }
}

void Engine::Init()
{
    mIsRunning=true;
    EventServer<PhaseChangedEvent>& PhaseChangeEventServer( EventServer<PhaseChangedEvent>::Get() );
    AutoReg PhaseChangeId(PhaseChangeEventServer.Subscribe( boost::bind( &Engine::OnPhaseChangedEvent, this, _1 ) ));
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
        if(!mIsRunning)
        {
            return;
        }
        it->mSystem->Update( DeltaTime );
    }
}

void Engine::AddSystem(int32_t Id)
{
    std::auto_ptr<System> sys = mSystemFactory(Id);
    int32_t typ = sys->GetType();
    mSystemHolder.mSystems.insert(SystemElement(typ,mSystemHolder.mSystems.size(),true, Opt<System>(sys.release())));
}

Engine::~Engine()
{
    for (Systems_t::const_iterator it = mSystemHolder.mSystems.begin(), e = mSystemHolder.mSystems.end();it!=e;++it)
    {
        delete &*it->mSystem;
    }
    mSystemHolder.mSystems.clear();
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
