#ifndef INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H

#include "message.h"
#include "engine/sub_system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"
#include "boost/ptr_container/ptr_vector.hpp"

using core::ProgramState;
namespace network {

class MessageHandlerSubSystem
{
protected:
    MessageHolder& mMessageHolder;
    Scene& mScene;
    ProgramState& mProgramState;
public:
    MessageHandlerSubSystem();
    virtual int32_t GetType() const = 0;
    virtual void Init() = 0;
    virtual void Update( double DeltaTime );
    virtual void Execute( Message const&  message ) = 0;
    virtual ~MessageHandlerSubSystem();
};

template<class T>
class PendingMessageHandlerSubSystem : public MessageHandlerSubSystem
{
protected:
    typedef std::vector<std::pair<T, int32_t> > PendingMessages_t;
    PendingMessages_t mPendingMessages;
public:
    PendingMessageHandlerSubSystem();
    virtual void Execute( Message const&  message );
    virtual void Update( double DeltaTime );
    virtual bool ProcessPending( Message const&  message ) = 0;
};

template<class T>
PendingMessageHandlerSubSystem<T>::PendingMessageHandlerSubSystem()
    : MessageHandlerSubSystem()
{

}

template<class T>
void PendingMessageHandlerSubSystem<T>::Update( double DeltaTime )
{
    for( typename PendingMessages_t::iterator i = mPendingMessages.begin(); i != mPendingMessages.end(); )
    {
        T const& msg = i->first;
        Opt<Actor> actor = mScene.GetActor( msg.mActorGUID );
        if ( !actor.IsValid() )
        {
            L2( "cannot find actor with GUID: %d, tries: %d (%s) (will try again later) \n", msg.mActorGUID, i->second, __FUNCTION__ );
            ++i->second;
            if ( i->second > 5 )
            {
                L1( "cannot find actor with GUID: %d, tries: %d (%s) (----remove message----) \n", msg.mActorGUID, i->second, __FUNCTION__ );
                i = mPendingMessages.erase( i );
            }
            else
            {
                ++i;
            }
            continue;
        }
        if ( i->second > 5 || ProcessPending( static_cast<Message const&>( msg ) ) )
        {
            i = mPendingMessages.erase( i );
        }
    }
}

template<class T>
void PendingMessageHandlerSubSystem<T>::Execute( Message const& message )
{
    T const& msg = static_cast<T const&>( message );
    mPendingMessages.push_back( std::make_pair<T const&, int32_t>( msg, 0 ) );
}



class DefaultMessageHandlerSubSystem : public MessageHandlerSubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( DefaultMessageHandlerSubSystem )
    virtual void Init();
    virtual void Execute( Message const&  message );
};
} // namespace engine

#endif//INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H
