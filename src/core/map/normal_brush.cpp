#include "normal_brush.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
namespace map {

NormalBrush::NormalBrush(int32_t Id)
    : IBrush(Id)
    , mMouseLeftPressed(false)
{

}

void NormalBrush::Update(double DeltaTime)
{
    if (mMouseLeftPressed&&!engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        std::auto_ptr<MapElement> mapElement(MapElementFactory::Get()(AutoId("spawn_actor")));
        Opt<SpawnActorMapElement> spawnActor(static_cast<SpawnActorMapElement*>(mapElement.get()));
        spawnActor->GetInputNodeId(SpawnActorMapElement::SpawnNodeId)(1);

        int32_t componentId=AutoId("position_component");
        ComponentLoaderFactory& componentLoaderFactory=ComponentLoaderFactory::Get();
        std::auto_ptr<ActorCreator::ComponentLoader_t> compLoader=componentLoaderFactory(componentId);
        Opt<PositionComponentLoader> positionCompLoader(static_cast<PositionComponentLoader*>(compLoader.get()));
        glm::vec2 curPos=EditorTargetSystem::Get()->GetCursorPosition();
        positionCompLoader->Bind<double>(&PositionComponent::SetX,curPos.x);
        positionCompLoader->Bind<double>(&PositionComponent::SetY,curPos.y);
        spawnActor->AddComponentLoader(componentId,compLoader);

        spawnActor->SetActorID(EditorTargetSystem::Get()->GetTarget().GetActorId());

        MapSystem::Get()->GetMapElementList().insert(Opt<MapElement>(mapElement.release()));
        mMouseLeftPressed=false;
    }
    else if (engine::Engine::Get().GetSystem<MouseSystem>()->IsButtonPressed( MouseSystem::Button_Left ))
    {
        mMouseLeftPressed=true;
    }
}

} // namespace map

