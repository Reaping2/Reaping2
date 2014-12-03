#include "platform/i_platform.h"
#include "core/remove_on_death_component.h"
#include "core/i_health_component.h"
#include "core/actor.h"
#include <boost/assert.hpp>

void RemoveOnDeathComponent::Update(double Seconds)
{
    BOOST_ASSERT(mActor && mActor->Get<IHealthComponent>().IsValid());

    if(!mNeedDelete&&!mActor->Get<IHealthComponent>()->IsAlive())
    {
        mNeedDelete=true;
    }
}

RemoveOnDeathComponent::RemoveOnDeathComponent()
    : mNeedDelete(false)
{

}

bool RemoveOnDeathComponent::NeedDelete()
{
    return mNeedDelete;
}


RemoveOnDeathComponentLoader::RemoveOnDeathComponentLoader()
{

}

void RemoveOnDeathComponentLoader::BindValues()
{

}
