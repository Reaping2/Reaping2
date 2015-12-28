#include "engine/buffs_engine/armor_buff_sub_system.h"
#include "core/buffs/armor_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_armor_component.h"
#include "core/buffs/buff_holder_component.h"

namespace engine {

ArmorBuffSubSystem::ArmorBuffSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void ArmorBuffSubSystem::Init()
{
}


void ArmorBuffSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if (!buffHolderC.IsValid())
    {
        return;
    }
    Opt<IArmorComponent> armorC=actor.Get<IArmorComponent>();
    if (!armorC.IsValid())
    {
        return;
    }

    BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffHolderC->GetBuffList(),ArmorBuff::GetType_static());
    if (buffListFilter.size()==0)
    {
        armorC->SetCurrentArmor(0);
    }
    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator armorBuffIt = buffListFilter.begin(), armorBuffE = buffListFilter.end(); armorBuffIt != armorBuffE; ++armorBuffIt )
    {
        Opt<ArmorBuff> armorBuff(*armorBuffIt);
        if (!armorBuff->IsApplied())
        {
            armorC->SetCurrentArmor(armorBuff->GetArmorMax());  //later there could be some improvements. Now there is only one armor buff
            armorBuff->SetApplied(true);
        }
    }
    if (armorC->GetCurrentArmor()==0)
    {
        for( BuffListFilter<IBuffHolderComponent::All>::const_iterator armorBuffIt = buffListFilter.begin(), armorBuffE = buffListFilter.end(); armorBuffIt != armorBuffE; ++armorBuffIt )
        {
            Opt<ArmorBuff> armorBuff(*armorBuffIt);
            buffHolderC->GetBuffList().modify(armorBuffIt,SecsToEndModifier(0.0));
        }
    }
}

} // namespace engine

