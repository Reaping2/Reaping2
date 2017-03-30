#include "select_target_act.h"
#include "platform/auto_id.h"
#include "core/i_target_holder_component.h"
#include "core/scene.h"
#include "core/i_health_component.h"
#include "core/i_position_component.h"
#include "distance_condition.h"

namespace scriptedcontroller
{

void SelectTargetAct::Update( Actor& actor, double Seconds )
{
    IAct::Update( actor, Seconds );
    if (!mTimer.IsTime() && !mTimer.Update( Seconds ))
    {
        return;
    }
    auto targetHolderC( actor.Get<ITargetHolderComponent>() );
    if (!targetHolderC.IsValid())
    {
        return;
    }
    for (auto const& selection : mSelections)
    {
        if (selection == SelectionType::Current)
        {
            if (SelectCurrentTarget( actor, *targetHolderC ))
            {
                L2( "Selected current target(%d) for actor: %d\n",targetHolderC->GetTargetGUID(),actor.GetGUID() );
                break;
            }
        }
        else if (selection == SelectionType::LastHitBy)
        {
            if (SelectLastHitByTarget( actor, *targetHolderC ))
            {
                L2( "Selected last_hit_by target(%d) for actor: %d\n", targetHolderC->GetTargetGUID(), actor.GetGUID() );
                break;
            }
        }
        else if (selection == SelectionType::Closest)
        {
            if (SelectClosestTarget( actor, *targetHolderC ))
            {
                L2( "Selected closest target(%d) for actor: %d\n", targetHolderC->GetTargetGUID(), actor.GetGUID() );
                break;
            }
        }
    }
}

void SelectTargetAct::Load( Json::Value const& setters )
{
    IAct::Load( setters );
    auto const& selections = setters["order_of_selection"];
    auto& selectionType(SelectionType::Get());
    if (selections.isArray())
    {
        for (auto const& selection : selections)
        {
            mSelections.push_back( selectionType( AutoId( selection.asString() ) ) );
        }
    }
    Json::GetBool( setters["seek_path"], mSeekPath );
    mTimer.Load( setters["frequency"] );
}

void SelectTargetAct::Start( Actor& actor )
{
    IAct::Start( actor );
    mTimer.Reset();
    mTimer.Update( mTimer.GetFrequency() );
}

void SelectTargetAct::Stop( Actor& actor )
{
    IAct::Stop( actor );
}


bool SelectTargetAct::SelectCurrentTarget( Actor& actor, ITargetHolderComponent& targetHolderC )
{
    auto target( mScene.GetActor( targetHolderC.GetTargetGUID() ) );
    if (!target.IsValid())
    {
        return false;
    }
    auto targetHealthC(target->Get<IHealthComponent>());
    if (!targetHealthC.IsValid() || !targetHealthC->IsAlive())
    {
        return false;
    }
    return true;
}


bool SelectTargetAct::SelectLastHitByTarget( Actor& actor, ITargetHolderComponent& targetHolderC )
{
    auto healthC( actor.Get<IHealthComponent>() );
    if (!healthC.IsValid())
    {
        return false;
    }
    if (mLastDamageTime == healthC->GetLastDamageTime())
    {
        return false;
    }
    mLastDamageTime = healthC->GetLastDamageTime();
    auto target( mScene.GetActor( healthC->GetLastDamageOwnerGUID() ) );
    if (!target.IsValid())
    {
        return false;
    }
    auto targetHealthC( target->Get<IHealthComponent>() );
    if (!targetHealthC.IsValid() || !targetHealthC->IsAlive())
    {
        return false;
    }
    targetHolderC.SetTargetGUID( target->GetGUID() );
    return true;
}


bool SelectTargetAct::SelectClosestTarget( Actor& actor, ITargetHolderComponent& targetHolderC )
{
    int32_t targetGuid = -1;
    double minDistance = std::numeric_limits<double>::max();
    auto positionC( actor.Get<IPositionComponent>() );
    BOOST_ASSERT( positionC.IsValid() );
    for (auto const& clientData : mProgramState.mClientDatas)
    {
        auto target( mScene.GetActor( clientData.mClientActorGUID ) );
        if (target.IsValid())
        {
            double distance = core::GetDistance( actor, *target, mSeekPath );
            if (distance < minDistance)
            {
                minDistance = distance;
                targetGuid = target->GetGUID();
            }
        }
    }
    if (targetGuid == -1)
    {
        return false;
    }
    targetHolderC.SetTargetGUID( targetGuid );
    return true;
}

} // namespace scriptedcontroller

