#include "platform/i_platform.h"
#include "core/actor.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/move_component.h"
#include "core/i_controller_component.h"
#include "platform/auto_id.h"
#include "inventory_component.h"
#include "core/i_renderable_component.h"
#include "core/renderable_layer.h"
#include "core/i_drop_on_death_component.h"
#include "core/i_remove_on_death_component.h"

using platform::AutoId;


Actor::Actor( int32_t Id )
    : mId( Id )
    , ComponentHolder ()
{
    static int32_t NextGuid = 0;
    mGUID = ++NextGuid;
}

Actor::Actor()
    : ComponentHolder()
    , mId(-1)
{

}


Actor::~Actor()
{
    int h=7;
}

int32_t Actor::GetId() const
{
    return mId;
}

void Actor::AddComponent(std::auto_ptr<Component> Comp)
{
    Comp->SetActorGUID(mGUID);
    ComponentHolder::AddComponent(Comp);
}

void Actor::SetId( int32_t Id )
{
    mId=Id;
}

void Actor::SetGUID(int32_t guid)
{
    mGUID=guid;
}

int32_t Actor::GetGUID() const
{
    return mGUID;
}
