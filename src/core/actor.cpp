#include "i_core.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/move_component.h"
#include "core/i_controller_component.h"
#include "platform/auto_id.h"
#include "inventory_component.h"
#include "core/collision_component.h"

using platform::AutoId;

void Actor::Update( double Seconds )
{
    for( ActionList_t::iterator i = mActions.begin(), e = mActions.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        i->second->Update( Seconds );
    }
    //TODO: maybe every component should have an update, and it should be called here
    //they might have priority order
    Opt<IInventoryComponent> inventoryC = Get<IInventoryComponent>();
    if(inventoryC.IsValid())
    {
        inventoryC->Update( Seconds );
    }
}

Actor::Actor( std::string const& Name )
    : AutoId( Name )
    , ComponentHolder ()
    , ActionHolder ()
{
    static int32_t NextGuid = 0;
    mGUID = ++NextGuid;
    AddAction( AutoId( "default_action" ) );
    AddComponent( mComponentFactory(AutoId("position_component")) );
    AddComponent( mComponentFactory(AutoId("move_component")) );
}


Actor::~Actor()
{
    int h=7;
}

void Actor::AddAction(int32_t Id)
{
    if( !CanAddAction( Id ) )
    {
        return;
    }
    std::auto_ptr<Action> a = mActionFactory( Id );
    AddAction(a);
}

void Actor::AddAction(std::auto_ptr<Action> Act)
{
    Act->SetActor(this);
    ActionHolder::AddAction(Act);
}


