#include "platform/i_platform.h"
#include "core/remove_on_death_component.h"
#include "core/i_health_component.h"
#include "core/actor.h"
#include <boost/assert.hpp>
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

RemoveOnDeathComponent::RemoveOnDeathComponent()
{

}

RemoveOnDeathComponentLoader::RemoveOnDeathComponentLoader()
{

}

void RemoveOnDeathComponentLoader::BindValues()
{

}

REAPING2_CLASS_EXPORT_IMPLEMENT(RemoveOnDeathComponent, RemoveOnDeathComponent);
