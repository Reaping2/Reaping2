#include "engine/buffs_engine/cloak_buff_sub_system.h"
#include "core/buffs/cloak_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_cloak_component.h"
#include "../cloak_changed_event.h"

namespace engine {

CloakBuffSubSystem::CloakBuffSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void CloakBuffSubSystem::Init()
{
}


void CloakBuffSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if (!buffHolderC.IsValid())
    {
        return;
    }

    Opt<ICloakComponent> cloakC=actor.Get<ICloakComponent>();
    if (!cloakC.IsValid())
    {
        return;
    }
    BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffHolderC->GetBuffList(),CloakBuff::GetType_static());
    if (buffListFilter.size()==0)
    {
        cloakC->SetActive(false);
        EventServer<engine::CloakChangedEvent>::Get().SendEvent(engine::CloakChangedEvent(actor.GetGUID(),false));
    }

    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator cloakBuffIt = buffListFilter.begin(), cloakBuffE = buffListFilter.end(); cloakBuffIt != cloakBuffE; ++cloakBuffIt )
    {
        Opt<CloakBuff> cloakBuff(*cloakBuffIt);
        if (!cloakBuff->IsApplied())
        {
            cloakC->SetActive(true);
            cloakBuff->SetApplied(true);
            EventServer<engine::CloakChangedEvent>::Get().SendEvent(engine::CloakChangedEvent(actor.GetGUID(),true));
        }
    }
}

} // namespace engine

