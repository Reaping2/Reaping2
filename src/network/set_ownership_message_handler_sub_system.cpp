#include "platform/i_platform.h"
#include "network/set_ownership_message_handler_sub_system.h"
#include "set_ownership_message.h"
#include "core/player_controller_component.h"
#include "core/opt.h"
#include "core/i_inventory_component.h"
#include "core/program_state.h"

namespace network {

    SetOwnershipMessageHandlerSubSystem::SetOwnershipMessageHandlerSubSystem()
        : mMessageHolder(MessageHolder::Get())
        , mScene(Scene::Get())
    {

    }

    void SetOwnershipMessageHandlerSubSystem::Init()
    {

    }

    void SetOwnershipMessageHandlerSubSystem::Execute(Message const& message)
    {
        SetOwnershipMessage const& msg=static_cast<SetOwnershipMessage const&>(message);
        L1("executing setownership: clientId %d \n",msg.mClientId );
        if (msg.mClientId!=core::ProgramState::Get().mClientId)
        {
            L1("thats not my id :( \n");
            return;
        }
        else
        {
            L1("thats my id! actorguid: %d \n",msg.mActorGUID);
        }
        Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);
        if (!actor.IsValid())
        {
            L1("cannot find actor with GUID: (that is not possible) %d \n",msg.mActorGUID );
            return;
        }

        Opt<PlayerControllerComponent> playerControllerC(actor->Get<PlayerControllerComponent>());
        if (!playerControllerC.IsValid())
        {
            L1("setownership is called on an actor that is not playerControllable \n" );
            return;
        }
        Opt<IInventoryComponent> inventoryC = actor->Get<IInventoryComponent>();
        inventoryC->AddItem(AutoId( "plasma_gun" ));
        inventoryC->SetSelectedWeapon(AutoId( "plasma_gun" ));
        playerControllerC->SetEnabled(true);
        mScene.SetPlayerModels(actor);
    }

} // namespace engine

