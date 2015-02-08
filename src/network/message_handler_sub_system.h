#ifndef INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H

#include "message.h"
#include "engine/sub_system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include "core/program_state.h"
using core::ProgramState;
namespace network {

    class MessageHandlerSubSystem
    {
    protected:
        MessageHolder& mMessageHolder;
        Scene& mScene;
        ProgramState& mProgramState;
        bool mIsClient;
        bool IsClient();
        bool mIsServer;
        bool IsServer();
    public:
        MessageHandlerSubSystem();
        virtual int32_t GetType() const=0;
        virtual void Init()=0;
        virtual void Update( double DeltaTime );
        virtual void Execute( Message const&  message )=0;
        virtual ~MessageHandlerSubSystem();
    };

    class DefaultMessageHandlerSubSystem : public MessageHandlerSubSystem
    {
    public:
        DEFINE_SUB_SYSTEM_BASE(DefaultMessageHandlerSubSystem)
        virtual void Init();
        virtual void Execute( Message const&  message );
    };
} // namespace engine

#endif//INCLUDED_NETWORK_MESSAGE_HANDLER_SUB_SYSTEM_H