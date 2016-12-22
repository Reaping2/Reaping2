#include "pickup_target.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "respawn_actor_map_element.h"
#include "../pickup_collision_component.h"
#include "../item_type.h"
#include "../i_position_component.h"
namespace map {

PickupTarget::PickupTarget(int32_t Id)
    : ITarget( Id )
    , mCursorId( Id )
    , mActorId( Id )
    , mScene( Scene::Get() )
{

}

PickupTarget::PickupTarget( int32_t Id, int32_t typeId, int32_t contentId )
    : ITarget( Id )
    , mCursorId( Id )
    , mActorId( Id )
    , mTypeId( typeId )
    , mContentId( contentId )
    , mScene( Scene::Get() )
{

}

void PickupTarget::Update( double DeltaTime )
{

}

int32_t PickupTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t PickupTarget::GetActorId() const
{
    return mActorId;
}

int32_t PickupTarget::GetTypeId() const
{
    return mTypeId;
}

void PickupTarget::PutTarget( glm::vec2 position )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "respawn_actor" ) ) );
    Opt<RespawnActorMapElement> spawnActor( static_cast<RespawnActorMapElement*>( mapElement.get() ) );
    spawnActor->GetInputNodeId( RespawnActorMapElement::SpawnNodeId() )( 1 );
    {
        int32_t componentId = AutoId( "position_component" );
        ComponentLoaderFactory& componentLoaderFactory = ComponentLoaderFactory::Get();
        std::auto_ptr<PropertyLoaderBase<Component> > compLoader = componentLoaderFactory( componentId );
        Opt<PositionComponentLoader> positionCompLoader( static_cast<PositionComponentLoader*>( compLoader.get() ) );
        positionCompLoader->Bind<double>( &PositionComponent::SetX, position.x );
        positionCompLoader->Bind<double>( &PositionComponent::SetY, position.y );
        spawnActor->AddComponentLoader( componentId, compLoader );
    }
    {
        int32_t componentId = AutoId( "pickup_collision_component" );
        ComponentLoaderFactory& componentLoaderFactory = ComponentLoaderFactory::Get();
        std::auto_ptr<PropertyLoaderBase<Component> > compLoader = componentLoaderFactory( componentId );
        Opt<PickupCollisionComponentLoader> pickupCompLoader( static_cast<PickupCollisionComponentLoader*>( compLoader.get() ) );
        pickupCompLoader->Bind<int32_t>( &PickupCollisionComponent::SetPickupContent, mContentId );
        pickupCompLoader->Bind<ItemType::Type>( &PickupCollisionComponent::SetItemType, ItemType::Get()( mTypeId ) );
        spawnActor->AddComponentLoader( componentId, compLoader );
    }
    spawnActor->SetActorID( EditorTargetSystem::Get()->GetTarget().GetActorId() );
    mapElement->SetIdentifier( EditorTargetSystem::Get()->GetNextUID() );
    spawnActor->SetSecsToRespawnOriginal( 15 );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}

std::auto_ptr<Actor> PickupTarget::GetCursor()
{
    std::auto_ptr<Actor> pickup( ActorFactory::Get()( mCursorId ) );
    Opt<PickupCollisionComponent> pickupCC( pickup->Get<ICollisionComponent>() );
    pickupCC->SetPickupContent( mContentId );
    pickupCC->SetItemType( ItemType::Get()( mTypeId ) );
    return pickup;
}

bool PickupTarget::Load( const Json::Value& setters )
{
    std::string type_id;
    if( !Json::GetStr(setters["type_id"], type_id))
    {
        L1("Error retrieving 'type_id' \n" );
        return false;
    }
    std::string content_id;
    if( !Json::GetStr(setters["content_id"], content_id))
    {
        L1("Error retrieving 'content_id' \n" );
        return false;
    }
    mTypeId = AutoId( type_id );
    mContentId = AutoId( content_id );
    return true;
}
} // namespace map
