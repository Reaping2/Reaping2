#include "platform/i_platform.h"
#include "spawn_soldiers_map_element_system.h"
#include "core/map/spawn_soldiers_map_element.h"
#include "engine/soldier_spawn_system.h"
#include "../player_controller_component.h"
#include "../i_position_component.h"

namespace map {

SpawnSoldiersMapElementSystem::SpawnSoldiersMapElementSystem()
    : MapElementSystem()
    , mProgramState( core::ProgramState::Get() )
{
}


void SpawnSoldiersMapElementSystem::Init()
{
    MapElementSystem::Init();
}


void SpawnSoldiersMapElementSystem::Update( double DeltaTime )
{
    MapElementSystem::Update( DeltaTime );
    MapElementListFilter<MapSystem::All> mapElementListFilter( mMapSystem->GetMapElementList(), SpawnSoldiersMapElement::GetType_static() );
    for( MapElementListFilter<MapSystem::All>::const_iterator spawnSoldiersMapElementIt = mapElementListFilter.begin(), spawnSoldiersMapElementE = mapElementListFilter.end(); spawnSoldiersMapElementIt != spawnSoldiersMapElementE; ++spawnSoldiersMapElementIt )
    {
        Opt<SpawnSoldiersMapElement> spawnSoldiersMapElement( *spawnSoldiersMapElementIt );
        if ( spawnSoldiersMapElement->GetValueId( SpawnSoldiersMapElement::SpawnNodeId() ) > 0 )
        {
            if ( mProgramState.mMode == core::ProgramState::Server )
            {
                Opt<engine::SoldierSpawnSystem> soldierSpawnS( engine::SoldierSpawnSystem::Get() );
                for ( core::ProgramState::ClientDatas_t::iterator i = mProgramState.mClientDatas.begin(), e = mProgramState.mClientDatas.end(); i != e; ++i )
                {
                    std::auto_ptr<Actor> player( soldierSpawnS->Spawn( *i ) );

                    mScene.AddActor( player.release() );
                }
            }
            else if ( mProgramState.mMode == core::ProgramState::Local )
            {
                Opt<core::ClientData> clientData( mProgramState.FindClientDataByClientId( mProgramState.mClientId ) );
                if ( clientData.IsValid() )
                {
                    L2( "spawning local player" );
                    clientData->mClientActorGUID = -1;
                    std::auto_ptr<Actor> Pl( engine::SoldierSpawnSystem::Get()->Spawn( *clientData ) );
                    Opt<PlayerControllerComponent>playerCC(Pl->Get<IControllerComponent>());
                    playerCC->SetEnabled( true );
                    playerCC->mActive = true;
                    // TODO: positionC?
                    auto positionC=(Pl->Get<IPositionComponent>());
                    mScene.SetPlayerModels( Opt<Actor>( Pl.get() ) );
                    mProgramState.mControlledActorGUID = clientData->mClientActorGUID;
                    mScene.AddActor( Pl.release() );
                }
            }
            L2( "spawn soldiers!\n" );
        }
        spawnSoldiersMapElement->ResetValues();
    }
}


} // namespace map

