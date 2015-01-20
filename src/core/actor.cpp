#include "platform/i_platform.h"
#include "core/actor.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/move_component.h"
#include "core/i_controller_component.h"
#include "platform/auto_id.h"
#include "inventory_component.h"
#include "core/i_renderable_component.h"
#include "core/action.h"
#include "core/action_holder.h"
#include "core/renderable_layer.h"
#include "core/i_drop_on_death_component.h"
#include "core/i_remove_on_death_component.h"

using platform::AutoId;

void Actor::Update( double Seconds )
{
    for( ActionList_t::iterator i = mActions.begin(), e = mActions.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        i->second->Update( Seconds );
    }
}

Actor::Actor( int32_t Id )
    : mId( Id )
    , ComponentHolder ()
    , ActionHolder ()
{
    static int32_t NextGuid = 0;
    mGUID = ++NextGuid;
    AddAction( AutoId( "default_action" ) );
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

int32_t Actor::GetId() const
{
    return mId;
}

void Actor::AddComponent(std::auto_ptr<Component> Comp)
{
    Comp->SetActor(this);
    ComponentHolder::AddComponent(Comp);
}

void Actor::SetId( int32_t Id )
{
    mId=Id;
}
