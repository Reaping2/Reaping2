#include "platform/i_platform.h"
#include "cloak_system.h"
#include "core/i_cloak_component.h"
#include "core/i_team_component.h"

namespace engine {

CloakSystem::CloakSystem()
    : mScene( Scene::Get() )
    , mProgramState(core::ProgramState::Get())
{
}


void CloakSystem::Init()
{
}


void CloakSystem::Update(double DeltaTime)
{
//     for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )
//     {
//        Actor& actor = **it;
//        Opt<ICloakComponent> cloakC=actor.Get<ICloakComponent>();
//        if (!cloakC.IsValid())
//        {
//            continue;
//        }
//     }
}

CloakSystem::CloakState CloakSystem::GetCloakState(const Actor& actor)
{
    Opt<ICloakComponent> cloakC=actor.Get<ICloakComponent>();
    if (!cloakC.IsValid()||!cloakC->IsActive())
    {
        return Visible;
    }
    static core::ProgramState& mProgramState = core::ProgramState::Get();
    if (mProgramState.mControlledActorGUID==actor.GetGUID())
    {
        return Cloaked;
    }
    Opt<ITeamComponent> teamC(actor.Get<ITeamComponent>());
    if (teamC.IsValid())
    {
        static Scene& mScene = Scene::Get();
        Opt<Actor> controlledActor=mScene.GetActor(mProgramState.mControlledActorGUID);
        if (controlledActor.IsValid())
        {
            Opt<ITeamComponent> controlledTeamC(controlledActor->Get<ITeamComponent>());
            if (controlledTeamC.IsValid()&&controlledTeamC->GetTeam()==teamC->GetTeam())
            {
                return Cloaked;
            } 
            else
            {
                return Invisible;
            }
        }
    }
    return Visible;
}


} // namespace engine

