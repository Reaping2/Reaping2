#include "platform/i_platform.h"
#include "network/set_ownership_message_handler_sub_system.h"
#include "set_ownership_message.h"
#include "core/player_controller_component.h"
#include "core/opt.h"
#include "core/i_inventory_component.h"
#include "core/program_state.h"

namespace network {

    SetOwnershipMessageHandlerSubSystem::SetOwnershipMessageHandlerSubSystem()
        : MessageHandlerSubSystem()
    {

    }

    void SetOwnershipMessageHandlerSubSystem::Init()
    {

    }

    void SetOwnershipMessageHandlerSubSystem::Execute(Message const& message)
    {
        SetOwnershipMessage const& msg=static_cast<SetOwnershipMessage const&>(message);
        L1("executing setownership: clientId %d \n",msg.mClientId );
        mPendingOwnerships.push_back(PendingOwnership(msg.mActorGUID,msg.mClientId));
    }

    void SetOwnershipMessageHandlerSubSystem::Update(double DeltaTime)
    {
        for(PendingOwnerships_t::iterator i=mPendingOwnerships.begin();i!=mPendingOwnerships.end();)
        {
            Opt<Actor> actor=mScene.GetActor(i->mActorGUID);
            if (!actor.IsValid())
            {
                L1("cannot find actor for setownership with GUID: %d (will try again later)\n",i->mActorGUID );
                ++i;
                continue;
            }

            Opt<PlayerControllerComponent> playerControllerC(actor->Get<PlayerControllerComponent>());
            if (!playerControllerC.IsValid())
            {
                L1("setownership is called on an actor that is not playerControllable \n" );
                i=mPendingOwnerships.erase(i);
                continue;
            }

            playerControllerC->mControllerId=i->mClientId;

            Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
            if (inventoryC.IsValid())
            {
                inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));
            }
            if (i->mClientId==mProgramState.mClientId)
            {
                L1("thats my id! actorguid: %d \n",i->mActorGUID);

                mProgramState.mControlledActorGUID=i->mActorGUID;
                playerControllerC->SetEnabled(true);
                playerControllerC->mActive=true;
                mScene.SetPlayerModels(actor);
            }
            i=mPendingOwnerships.erase(i);
        }
    }

} // namespace engine

