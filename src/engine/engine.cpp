#include "platform/i_platform.h"
#include "engine/engine.h"
#include "core/perf_timer.h"

namespace engine {

Engine::Engine()
    : mSystemFactory( SystemFactory::Get() )
    , mIsRunning( false )
{
}
void Engine::OnPhaseChangedEvent( PhaseChangedEvent const& Evt )
{
    if( Evt.CurrentPhase == ProgramPhase::InitiateShutDown )
    {
        mIsRunning = false;
    }
}

void Engine::Init()
{
    mIsRunning = true;
    EventServer<PhaseChangedEvent>& PhaseChangeEventServer( EventServer<PhaseChangedEvent>::Get() );
    AutoReg PhaseChangeId( PhaseChangeEventServer.Subscribe( boost::bind( &Engine::OnPhaseChangedEvent, this, _1 ) ) );
    for ( Systems_t::const_iterator it = mSystemHolder.mSystems.begin(), e = mSystemHolder.mSystems.end(); it != e; ++it )
    {
        it->mSystem->Init();
    }
}

void Engine::Update( double DeltaTime )
{
    if (!mIsRunning)
    {
        return;
    }
    if (mSuppressed)
    {
        DeltaTime = 0.0;
    }
    static perf::Timer_t mPerfTimer;
    SystemsFilter<Engine::EnabledSystems> enabledSystems( mSystemHolder.mSystems );
    for ( SystemsFilter<Engine::EnabledSystems>::const_iterator it = enabledSystems.begin(), e = enabledSystems.end(); it != e; ++it )
    {
        auto& idStorage = IdStorage::Get();
        std::string sysName;
        if (idStorage.GetName( it->mSystem->GetType(), sysName ))
        {
            L2( "System update: %s\n", sysName.c_str() );
        }
        mPerfTimer.Log( "sys start" );
        it->mSystem->Update( DeltaTime );
        mPerfTimer.Log( "sys end" );
    }
}

void Engine::AddSystem( int32_t Id )
{
    std::auto_ptr<System> sys = mSystemFactory( Id );
    int32_t typ = sys->GetType();
    mSystemHolder.mSystems.insert( SystemElement( typ, mSystemHolder.mSystems.size(), true, Opt<System>( sys.release() ) ) );
}

Engine::~Engine()
{
    for ( Systems_t::const_iterator it = mSystemHolder.mSystems.begin(), e = mSystemHolder.mSystems.end(); it != e; ++it )
    {
        delete &*it->mSystem;
    }
    mSystemHolder.mSystems.clear();
}

void Engine::SetEnabled( int32_t systemType, bool enabled )
{
    Systems_t::iterator it = mSystemHolder.mSystems.find( systemType );
    if (it != mSystemHolder.mSystems.end())
    {
        mSystemHolder.mSystems.modify( it, SystemEnableModifier( enabled ) );
    }
}

bool Engine::IsEnabled( int32_t systemType ) const
{
    Systems_t::iterator it = mSystemHolder.mSystems.find( systemType );
    return it != mSystemHolder.mSystems.end() ? it->mSystem->IsEnabled() : false;
}

void Engine::OnSuppressEvent( SuppressEvent const& evt )
{
    mSuppressed = evt.mSuppressed;
}




int32_t SystemHolder::SystemDefaultOrderer::operator()( const SystemElement& system ) const
{
    return system.mOrder;
}


int32_t SystemHolder::SystemIdOrderer::operator()( const SystemElement& system ) const
{
    return system.mId;
}


bool SystemHolder::IsEnabled::operator()( const SystemElement& system ) const
{
    return system.mEnabled;
}


SystemElement::SystemElement( int32_t id, int32_t order, bool enabled, Opt<System> system )
    : mId( id )
    , mOrder( order )
    , mEnabled( enabled )
    , mSystem( system )
{

}


SystemEnableModifier::SystemEnableModifier( bool enabled )
    : mEnabled( enabled )
{

}

void SystemEnableModifier::operator()( SystemElement& system )
{
    system.mEnabled = mEnabled;
    system.mSystem->SetEnabled( mEnabled );
}

} // namespace engine
