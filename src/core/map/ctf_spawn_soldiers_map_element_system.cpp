#include "platform/i_platform.h"
#include "ctf_spawn_soldiers_map_element_system.h"
#include "core/map/ctf_spawn_soldiers_map_element.h"
#include "core/player_controller_component.h"
#include "engine/ctf_soldier_spawn_system.h"

namespace map {
namespace ctf {

CtfSpawnSoldiersMapElementSystem::CtfSpawnSoldiersMapElementSystem()
    : MapElementSystem()
    , mProgramState( ::core::ProgramState::Get() )
    , mCtfProgramState( ::ctf::ProgramState::Get() )
{
}


void CtfSpawnSoldiersMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void CtfSpawnSoldiersMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), CtfSpawnSoldiersMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator ctfSpawnSoldiersMapElementIt = mapElementListFilter.begin(), ctfSpawnSoldiersMapElementE = mapElementListFilter.end(); ctfSpawnSoldiersMapElementIt != ctfSpawnSoldiersMapElementE; ++ctfSpawnSoldiersMapElementIt )
    {
        Opt<CtfSpawnSoldiersMapElement> ctfSpawnSoldiersMapElement( *ctfSpawnSoldiersMapElementIt );
        if ( ctfSpawnSoldiersMapElement->GetValueId( CtfSpawnSoldiersMapElement::SpawnNodeId() ) > 0 )
        {
            if ( mProgramState.mMode != core::ProgramState::Client )
            {
                Opt<engine::ctf::CtfSoldierSpawnSystem> ctfSoldierSpawnS( engine::ctf::CtfSoldierSpawnSystem::Get() );
                for ( core::ProgramState::ClientDatas_t::iterator i = mProgramState.mClientDatas.begin(), e = mProgramState.mClientDatas.end(); i != e; ++i )
                {
                    Opt< ::ctf::ClientData> ctfClientData( mCtfProgramState.FindClientDataByClientId( i->mClientId ) );
                    if ( ctfClientData.IsValid() )
                    {
                        std::auto_ptr<Actor> player( ctfSoldierSpawnS->Spawn( *i, ctfClientData->mTeam ) );
                        mScene.AddActor( player.release() );
                    }
                }
            }
            L2( "ctf_spawn soldiers!" );
        }
        ctfSpawnSoldiersMapElement->ResetValues();
    }
}

} // namespace ctf
} // namespace map

