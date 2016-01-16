#include "engine/buffs_engine/move_speed_buff_sub_system.h"
#include "core/buffs/move_speed_buff.h"
#include "core/buffs/i_buff_holder_component.h"
#include "core/i_move_component.h"

namespace engine {

MoveSpeedBuffSubSystem::MoveSpeedBuffSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
{
}


void MoveSpeedBuffSubSystem::Init()
{
}


void MoveSpeedBuffSubSystem::Update(Actor& actor, double DeltaTime)
{
    RecalculateBuffs(actor);
}

void MoveSpeedBuffSubSystem::RecalculateBuffs(Actor &actor)
{
    Opt<IBuffHolderComponent> buffHolderC = actor.Get<IBuffHolderComponent>();
    if (!buffHolderC.IsValid())
    {
        return;
    }
    Opt<IMoveComponent> moveC=actor.Get<IMoveComponent>();
    if(!moveC.IsValid())
    {
        return;
    }
    moveC->GetSpeed().mFlat.Set(0.0);
    moveC->GetSpeed().mPercent.Set(0.0);
    bool rooted = false;
    BuffListFilter<IBuffHolderComponent::All> buffListFilter(buffHolderC->GetBuffList(),MoveSpeedBuff::GetType_static());
    for( BuffListFilter<IBuffHolderComponent::All>::const_iterator moveSpeedBuffIt = buffListFilter.begin(), moveSpeedBuffE = buffListFilter.end(); moveSpeedBuffIt != moveSpeedBuffE; ++moveSpeedBuffIt )
    {
        Opt<MoveSpeedBuff> moveSpeedBuff(*moveSpeedBuffIt);
        moveC->GetSpeed().mFlat.Set(moveC->GetSpeed().mFlat.Get()+moveSpeedBuff->GetFlatBonus());
        moveC->GetSpeed().mPercent.Set(moveC->GetSpeed().mPercent.Get()+moveSpeedBuff->GetPercentBonus());
        rooted = rooted || moveSpeedBuff->IsRooted();
    }
    moveC->SetRooted( rooted );
 }

} // namespace engine

