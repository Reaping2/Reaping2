#include "platform/i_platform.h"
#include "ctf_spawn_flags_map_element_system.h"
#include "core/map/ctf_spawn_flags_map_element.h"
#include "ctf_flag_spawn_point_map_element.h"
#include "../i_team_component.h"
#include "../i_position_component.h"
#include "engine/flag_created_event.h"

namespace map {
namespace ctf {

CtfSpawnFlagsMapElementSystem::CtfSpawnFlagsMapElementSystem()
    : MapElementSystem()
    , mProgramState(::core::ProgramState::Get())
    , mActorFactory( ActorFactory::Get() )
    , mFlagAutoId(AutoId("flag"))
{
}


void CtfSpawnFlagsMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void CtfSpawnFlagsMapElementSystem::Update(double DeltaTime)
{
    MapElementSystem::Update(DeltaTime);
    MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),CtfSpawnFlagsMapElement::GetType_static());
    for( MapElementListFilter<MapSystem::All>::const_iterator ctfSpawnFlagsMapElementIt = mapElementListFilter.begin(), ctfSpawnFlagsMapElementE = mapElementListFilter.end(); ctfSpawnFlagsMapElementIt != ctfSpawnFlagsMapElementE; ++ctfSpawnFlagsMapElementIt )
    {
        Opt<CtfSpawnFlagsMapElement> ctfSpawnFlagsMapElement(*ctfSpawnFlagsMapElementIt);
        if (ctfSpawnFlagsMapElement->GetValueId(CtfSpawnFlagsMapElement::SpawnNodeId)>0)
        {
            if (mProgramState.mMode==core::ProgramState::Server)
            {
                MapElementListFilter<MapSystem::All> mapElementListFilter(mMapSystem->GetMapElementList(),CtfFlagSpawnPointMapElement::GetType_static());
                for( MapElementListFilter<MapSystem::All>::const_iterator ctfFlagSpawnPointMapElementIt = mapElementListFilter.begin(), ctfFlagSpawnPointMapElementE = mapElementListFilter.end(); ctfFlagSpawnPointMapElementIt != ctfFlagSpawnPointMapElementE; ++ctfFlagSpawnPointMapElementIt )
                {
                    Opt<CtfFlagSpawnPointMapElement> ctfFlagSpawnPointMapElement(*ctfFlagSpawnPointMapElementIt);
                    std::auto_ptr<Actor> ctfFlag(mActorFactory(mFlagAutoId));
                    Opt<ITeamComponent> teamC(ctfFlag->Get<ITeamComponent>());
                    if (teamC.IsValid())
                    {
                        teamC->SetTeam(ctfFlagSpawnPointMapElement->GetTeam());
                    }
                    Opt<IPositionComponent> positionC = ctfFlag->Get<IPositionComponent>();
                    if (positionC.IsValid())
                    {
                        positionC->SetX(ctfFlagSpawnPointMapElement->GetX());
                        positionC->SetY(ctfFlagSpawnPointMapElement->GetY());
                    }
                    EventServer<engine::FlagCreatedEvent>::Get().SendEvent(engine::FlagCreatedEvent(Opt<Actor>(ctfFlag.get())));

                    mScene.AddActor(ctfFlag.release());                   
                }
            }
            else if (mProgramState.mMode==core::ProgramState::Local)
            {
                // no local support for ctf
            }
            L1("spawn flags!");
        }
        ctfSpawnFlagsMapElement->ResetValues();
    }
}

} // namespace ctf
} // namespace map

