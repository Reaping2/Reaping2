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
namespace map {

WallTarget::WallTarget(int32_t Id, int32_t cursorId, int32_t actorId)
    : ITarget(Id)
    , mCursorId(cursorId)
    , mActorId(actorId)
    , mScene(Scene::Get())
{

}

void WallTarget::Update(double DeltaTime)
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

void WallTarget::PutTarget(glm::vec2 position)
{
    std::auto_ptr<MapElement> mapElement(MapElementFactory::Get()(AutoId("spawn_actor")));
    Opt<SpawnActorMapElement> spawnActor(static_cast<SpawnActorMapElement*>(mapElement.get()));
    spawnActor->GetInputNodeId(SpawnActorMapElement::SpawnNodeId)(1);

    int32_t componentId=AutoId("position_component");
    ComponentLoaderFactory& componentLoaderFactory=ComponentLoaderFactory::Get();
    std::auto_ptr<ActorCreator::ComponentLoader_t> compLoader=componentLoaderFactory(componentId);
    Opt<PositionComponentLoader> positionCompLoader(static_cast<PositionComponentLoader*>(compLoader.get()));
    positionCompLoader->Bind<double>(&PositionComponent::SetX,position.x);
    positionCompLoader->Bind<double>(&PositionComponent::SetY,position.y);
    spawnActor->AddComponentLoader(componentId,compLoader);

    spawnActor->SetActorID(EditorTargetSystem::Get()->GetTarget().GetActorId());
    mapElement->SetUID(AutoId("spawn_at_start"));
    MapSystem::Get()->GetMapElementList().insert(Opt<MapElement>(mapElement.release()));
}

} // namespace map
