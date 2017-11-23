#include "wall_target.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "../border_component.h"
namespace map {

WallTarget::WallTarget(int32_t Id)
    : ITarget( Id )
    , mCursorId( -1 )
    , mActorId( -1 )
    , mScene( Scene::Get() )
{

}

WallTarget::WallTarget( int32_t Id, int32_t cursorId, int32_t actorId )
    : ITarget( Id )
    , mCursorId( cursorId )
    , mActorId( actorId )
    , mScene( Scene::Get() )
{

}

void WallTarget::Update( double DeltaTime )
{

}

int32_t WallTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t WallTarget::GetActorId() const
{
    return mActorId;
}

void WallTarget::PutTarget( glm::vec2 position )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "spawn_actor" ) ) );
    Opt<SpawnActorMapElement> spawnActor( static_cast<SpawnActorMapElement*>( mapElement.get() ) );
    spawnActor->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() )( 1 );

    AddPositionLoader( position, spawnActor );

    spawnActor->SetActorID( EditorTargetSystem::Get()->GetTarget().GetActorId() );
    mapElement->SetIdentifier( EditorTargetSystem::Get()->GetNextUID() );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}

void WallTarget::PutTarget( glm::vec2 position, IBorderComponent::Borders_t& borders, IBorderComponent::Borders_t& outerBorders )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "spawn_actor" ) ) );
    Opt<SpawnActorMapElement> spawnActor( static_cast<SpawnActorMapElement*>( mapElement.get() ) );
    spawnActor->GetInputNodeId( SpawnActorMapElement::SpawnNodeId() )( 1 );

    AddPositionLoader( position, spawnActor );

    AddBorderLoader( borders, outerBorders, spawnActor );



    spawnActor->SetActorID( EditorTargetSystem::Get()->GetTarget().GetActorId() );
    mapElement->SetIdentifier( EditorTargetSystem::Get()->GetNextUID() );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
}

bool WallTarget::Load( const Json::Value& setters )
{
    std::string cursor_id;
    if( !Json::GetStr(setters["cursor_id"], cursor_id))
    {
        L1("Error retrieving 'cursor_id' \n" );
        return false;
    }
    std::string actor_id;
    if( !Json::GetStr(setters["actor_id"], actor_id))
    {
        L1("Error retrieving 'actor_id' \n" );
        return false;
    }
    L1( "WallTarget loaded: actorId:%s,cursorId:%s\n", actor_id.c_str(), cursor_id.c_str() );
    mCursorId = AutoId( cursor_id );
    mActorId = AutoId( actor_id );
    return true;
}

void WallTarget::AddPositionLoader( glm::vec2& position, Opt<SpawnActorMapElement> spawnActor )
{
    spawnActor->AddComponentLoader( AutoId( "position_component" ),
        std::move( PositionComponentLoader::FromPosition( position.x, position.y ) ) );
}

void WallTarget::AddBorderLoader( IBorderComponent::Borders_t& borders, IBorderComponent::Borders_t& outerBorders, Opt<SpawnActorMapElement> spawnActor )
{
    {
        int32_t componentId = AutoId( "border_component" );
        ComponentLoaderFactory& componentLoaderFactory = ComponentLoaderFactory::Get();
        std::unique_ptr<PropertyLoaderBase<Component> > compLoader = componentLoaderFactory( componentId );
        Opt<BorderComponentLoader> borderCompLoader( static_cast<BorderComponentLoader*>( compLoader.get() ) );
        borderCompLoader->Bind<IBorderComponent::Borders_t>( &BorderComponent::SetBorders, borders );
        borderCompLoader->Bind<IBorderComponent::Borders_t>( &BorderComponent::SetOuterBorders, outerBorders );
        spawnActor->AddComponentLoader( componentId, std::move(compLoader) );
    }
}

} // namespace map
