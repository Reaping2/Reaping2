#include "platform/i_platform.h"
#include "core/drop_on_death_component.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "core/scene.h"
#include "core/actor_factory.h"
#include "platform/auto_id.h"
#include <boost/assert.hpp>
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

using platform::AutoId;

void DropOnDeathComponent::Update(double Seconds)
{
}

DropOnDeathComponent::DropOnDeathComponent()
    : mTriedDrop(false)
{

}

bool DropOnDeathComponent::IsTriedDrop()
{
    return mTriedDrop;
}

void DropOnDeathComponent::SetTriedDrop(bool triedDrop)
{
    mTriedDrop=triedDrop;
}

DropOnDeathComponentLoader::DropOnDeathComponentLoader()
{

}

void DropOnDeathComponentLoader::BindValues()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT(DropOnDeathComponent, DropOnDeathComponent);
