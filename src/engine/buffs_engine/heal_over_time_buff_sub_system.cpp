#include "platform/i_platform.h"
#include "engine/buffs_engine/heal_over_time_buff_sub_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/buffs/heal_over_time_buff.h"
#include "core/i_health_component.h"

namespace engine {

HealOverTimeBuffSubSystem::HealOverTimeBuffSubSystem()
    : mScene( Scene::Get() )
    , mProgramState (core::ProgramState::Get())
{

}

void HealOverTimeBuffSubSystem::Init()
{


}

void HealOverTimeBuffSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if (!buffHolderC.IsValid())
    {
        return;
    }
    Opt<IHealthComponent> healthC = actor.Get<IHealthComponent>();
    if (!healthC.IsValid()||!healthC->IsAlive())
    {
        return;
    }

    if (mProgramState.mMode!=core::ProgramState::Client)
    {
        BuffList_t& buffList=buffHolderC->GetBuffList();
        BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffList,HealOverTimeBuff::GetType_static());
        for( BuffListFilter<IBuffHolderComponent::All>::const_iterator healOverTimeBuffIt = buffListFilter.begin(), healOverTimeBuffE = buffListFilter.end(); healOverTimeBuffIt != healOverTimeBuffE; ++healOverTimeBuffIt )
        {
            Opt<Buff> b=*healOverTimeBuffIt;
            Opt<HealOverTimeBuff> healOverTimeBuff(b);
            healOverTimeBuff->GetFrequencyTimer().Update(DeltaTime);
            healthC->TakeHeal(healOverTimeBuff->GetFrequencyTimer().GetElapsedTicks()*healOverTimeBuff->GetHealPerTick());
        }

    }
}

} // namespace engine

