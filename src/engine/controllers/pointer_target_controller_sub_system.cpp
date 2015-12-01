#include "platform/i_platform.h"
#include "engine/engine.h"
#include "engine/controllers/pointer_target_controller_sub_system.h"
#include "core/pointer_target_controller_component.h"
#include "core/i_target_holder_component.h"
#include "core/i_listen_child_death_component.h"
#include "core/i_notify_parent_on_death_component.h"
#include "../items/weapon_item_sub_system.h"
#include "core/i_move_component.h"
#include "target_player_controller_sub_system.h"
#include "core/i_position_component.h"
#include "core/shot_collision_component.h"
#include "core/i_health_component.h"

namespace engine {

PointerTargetControllerSubSystem::PointerTargetControllerSubSystem()
    : mScene(Scene::Get())
    , mProgramState(core::ProgramState::Get())
    , mActorFactory(ActorFactory::Get())
    , mLaserPointerProjectileId(AutoId("laser_pointer_projectile"))
{
}


void PointerTargetControllerSubSystem::Init()
{
    mRemovedActorsSystem=Engine::Get().GetSystem<RemovedActorsSystem>();
}


void PointerTargetControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<PointerTargetControllerComponent> pointerTargetCC=actor.Get<PointerTargetControllerComponent>();
    if (!pointerTargetCC.IsValid()||!pointerTargetCC->IsEnabled())
    {
        return;
    }
    actor.Get<IPositionComponent>()->SetOrientation( actor.Get<IMoveComponent>()->GetHeading() );

    if (mProgramState.mMode==core::ProgramState::Client)
    {
        return;
    }
    Opt<ITargetHolderComponent> targetHolderC=actor.Get<ITargetHolderComponent>();
    Opt<IListenChildDeathComponent> listenChildDeathC=actor.Get<IListenChildDeathComponent>();

    Opt<Actor> pointedTarget(mScene.GetActor(pointerTargetCC->GetPointedTargetGUID()));
    Opt<Actor> killerOfChild(mScene.GetActor(listenChildDeathC->GetKillerOfChildGUID()));
    Opt<Actor> currentTarget(mScene.GetActor(targetHolderC->GetTargetGUID()));
    bool currentTargetIsAlive=false;
    if(currentTarget.IsValid())
    {
        Opt<IHealthComponent> currentTargetHealthC=currentTarget->Get<IHealthComponent>();
        currentTargetIsAlive=!currentTargetHealthC.IsValid()||currentTargetHealthC->IsAlive();
    }
    Opt<ShotCollisionComponent> shotCC(actor.Get<ShotCollisionComponent>());
    if (shotCC.IsValid()&&killerOfChild.IsValid()&&killerOfChild->GetGUID()==shotCC->GetParentGuid())
    {
        listenChildDeathC->SetKillerOfChildGUID(-1);
        killerOfChild.Reset(); //just to be sure
        return;
    }
    if (!targetHolderC.IsValid()||!listenChildDeathC.IsValid())
    {
        L1("ERROR -there should be target_holder and listen_child_death if pointer_target_controller is used!");
        return;
    }
    if(killerOfChild.IsValid())
    {
        if(pointedTarget.IsValid()&&pointedTarget->GetGUID()==killerOfChild->GetGUID())
        {
            pointerTargetCC->SetPointedTargetCounter(pointerTargetCC->GetPointedTargetCounter()+1);
        }
        else
        {
            pointerTargetCC->SetPointedTargetGUID(killerOfChild->GetGUID());
            pointerTargetCC->SetPointedTargetCounter(0);
            pointedTarget=mScene.GetActor(pointerTargetCC->GetPointedTargetGUID());
        }
    }
    if(!pointedTarget.IsValid())
    {
        pointerTargetCC->SetPointedTargetCounter(0);
    }

    if((!currentTarget.IsValid()||!currentTargetIsAlive)&&pointedTarget.IsValid()||pointerTargetCC->GetPointedTargetCounter()>=2)
    {
        targetHolderC->SetTargetGUID(pointedTarget->GetGUID());
        currentTarget=mScene.GetActor(targetHolderC->GetTargetGUID());
    }
    listenChildDeathC->SetKillerOfChildGUID(-1);
    killerOfChild.Reset(); //just to be sure

    pointerTargetCC->SetNextLaserCounter(pointerTargetCC->GetNextLaserCounter() - DeltaTime);
    if(pointerTargetCC->GetNextLaserCounter()<=0.0)
    {
        pointerTargetCC->SetNextLaserCounter(pointerTargetCC->GetNextLaserFrequency());
        std::auto_ptr<Actor> laserPointer = mActorFactory(mLaserPointerProjectileId);
        Opt<INotifyParentOnDeathComponent> notifyParentC=laserPointer->Get<INotifyParentOnDeathComponent>();
        if(notifyParentC.IsValid())
        {
            notifyParentC->SetParentGUID(actor.GetGUID());
        }
        WeaponItemSubSystem::Projectiles_t projectiles;
        projectiles.push_back(laserPointer.release());
        Scatter scatter;
        WeaponItemSubSystem::Get()->AddProjectiles(actor,projectiles,scatter);
    }

    if (currentTarget.IsValid())
    {
        double Radians=TargetPlayerControllerSubSystem::GetRotationDiffRadians(actor,*currentTarget);

        pointerTargetCC->SetHeadingModifierCounter(pointerTargetCC->GetHeadingModifierCounter() - DeltaTime);
        if(pointerTargetCC->GetHeadingModifierCounter()<=0.0)
        {
            pointerTargetCC->SetHeadingModifierCounter(pointerTargetCC->GetHeadingModifierFrequency());
            actor.Get<IMoveComponent>()->SetHeadingModifier( (Radians>0?1:-1)*2.5 );
        }
        if(std::abs(Radians)<0.1)
        {
            actor.Get<IMoveComponent>()->SetHeadingModifier( 0.0 );
        }
    }
}
// {
//     "name":"laser_pointer_projectile",
//         "recognizers":
//     [
//     {"recognizer":"move",
//     "renderer":"move"}
//     ]
// },	

} // namespace engine

