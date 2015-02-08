#ifndef INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
#define INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
#include "message_handler_sub_system.h"
#include "messsage_holder.h"
#include "core/scene.h"
#include <vector>

namespace network {

    struct PendingOwnership
    {
        int32_t mActorGUID;
        int32_t mClientId; //TODO: temporary this is for inventory hack
        PendingOwnership(int32_t actorGUID, int32_t clientId) :mActorGUID(actorGUID),mClientId(clientId){}
    };

    class SetOwnershipMessageHandlerSubSystem: public MessageHandlerSubSystem
    {
        typedef std::vector<PendingOwnership> PendingOwnerships_t;
        PendingOwnerships_t mPendingOwnerships;
    public:
        DEFINE_SUB_SYSTEM_BASE(SetOwnershipMessageHandlerSubSystem)
        SetOwnershipMessageHandlerSubSystem();
        virtual void Init();
        virtual void Update( double DeltaTime );

        virtual void Execute(Message const& message );
    };

} // namespace network

#endif//INCLUDED_NETWORK_SET_OWNERSHIP_MESSAGE_HANDLER_SUB_SYSTEM_H
