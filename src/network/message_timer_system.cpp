#include "platform/i_platform.h"
#include "message_timer_system.h"
#include "core/program_state.h"
#include "../platform/filesystem_utils.h"

namespace network {

MessageTimerSystem::MessageTimerSystem()
    : mScene( Scene::Get() )
{
}


void MessageTimerSystem::Init()
{
    fs_utils::for_each( "actors", ".network", [&]( Json::Value const& desc )
    {
        std::string messageName;
        if (!Json::GetStr( desc["message_name"], messageName ))
        {
            return;
        }
        int32_t messageId = AutoId( messageName );
        auto& messageTimer = mMessageTimers[messageId];
        messageTimer.Load( desc );
    } );
}

void MessageTimerSystem::Update(double DeltaTime)
{
    for (auto& messageTimer : mMessageTimers)
    {
        messageTimer.second.Update( DeltaTime );
    }
}

Opt<MessageTimer> MessageTimerSystem::GetMessageTimer( int32_t mMessageJsonId )
{
    auto it = mMessageTimers.find( mMessageJsonId );
    return it == mMessageTimers.end() ? nullptr : &it->second;
}


ActorFrequencyTimerHolder const& MessageTimer::GetUnique() const
{
    return mUniqueFrequencyTimerHolder;
}

ActorFrequencyTimerHolder const& MessageTimer::GetMandatory() const
{
    return mMandatoryFrequencyTimerHolder;
}

void MessageTimer::Load( Json::Value const& setters )
{
    static auto& mProgramState( core::ProgramState::Get() );
    std::string messageName;
    if (!Json::GetStr( setters["message_name"], messageName ))
    {
        return;
    }
    std::string name;
    if (!Json::GetStr( setters["name"], name ))
    {
        return;
    }
    mId = AutoId( messageName );
    int32_t actorId = AutoId( name );
    Json::Value freqJson = setters[mProgramState.mMode == core::ProgramState::Server?"server":"client"];
    if (!freqJson.isObject())
    {
        return;
    }
    double freq = 0.0;
    if (Json::GetDouble( freqJson["unique_frequency"], freq )
        && freq > -1)
    {
            mUniqueFrequencyTimerHolder.Add( { freq,actorId } );
    }
    if (Json::GetDouble( freqJson["mandatory_frequency"], freq )
        && freq > -1)
    {
        mMandatoryFrequencyTimerHolder.Add( { freq,actorId } );
    }

}

void MessageTimer::Update( double DeltaTime )
{
    mUniqueFrequencyTimerHolder.Update( DeltaTime );
    mMandatoryFrequencyTimerHolder.Update( DeltaTime );
}

} // namespace network

