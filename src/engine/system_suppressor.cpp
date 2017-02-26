#include "engine/system_suppressor.h"
#include "platform/i_platform.h"
#include "engine.h"

namespace engine {

SystemSuppressor::SystemSuppressor()
{
}

void SystemSuppressor::Add( SystemSuppressor::Type type, int32_t systemType )
{
    mTypeToSystems[type].insert( systemType );
}

bool SystemSuppressor::Suppress( SystemSuppressor::Type type, bool takeOwnership /*= false */ )
{
    if ( mOwnerSystemType != None && !takeOwnership )
    {
        return false;
    }
    if ( type == mOwnerSystemType )
    {
        return true;
    }
    ResumeSystemStates();
    mSystemStates.clear();
    mOwnerSystemType = type;
    static engine::Engine& mEngine = engine::Engine::Get();

    auto& systems( mTypeToSystems[type] );
    for (auto& sysId : systems)
    {
        mSystemStates[sysId] = mEngine.IsEnabled( sysId );
        if (mSystemStates[sysId])
        {
            mEngine.SetEnabled( sysId, false );
        }
    }
    L2( "Systems suspended %d\n", type );
    platform::EventServer<SuppressEvent>::Get().SendEvent( { type, true } );
    return true;
}

bool SystemSuppressor::Resume( SystemSuppressor::Type type )
{
    if (type != Force && type != mOwnerSystemType)
    {
        return false;
    }
    ResumeSystemStates();

    mSystemStates.clear();
    mOwnerSystemType = None;
    L2( "Systems resumed %d\n", type );
    platform::EventServer<SuppressEvent>::Get().SendEvent( { type, false } );
    return true;
}

bool SystemSuppressor::IsSuppressed() const
{
    return mOwnerSystemType != None;
}

engine::SystemSuppressor::Type SystemSuppressor::GetOwnerSystemType() const
{
    return mOwnerSystemType;
}

void SystemSuppressor::ResumeSystemStates()
{
    static engine::Engine& mEngine = engine::Engine::Get();
    for (auto& systemState : mSystemStates)
    {
        mEngine.SetEnabled( systemState.first, systemState.second );
    }
}

} // namespace engine

