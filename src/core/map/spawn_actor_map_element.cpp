#include "core/map/spawn_actor_map_element.h"
#include "../scene.h"
#include "../i_position_component.h"
#include "../i_border_component.h"

namespace map {

const int SpawnActorMapElement::SpawnNodeId=AutoId("spawn");

SpawnActorMapElement::SpawnActorMapElement(int32_t Id)
    : MapElement(Id)
    , BaseInput()
    , mActorID(-1)
    , mComponentLoaderFactory(ComponentLoaderFactory::Get())
{
    AddInputNodeId(SpawnNodeId);
}

void SpawnActorMapElement::Load(Json::Value& setters)
{
    MapElement::Load(setters);

    std::string actorStr;
    if (!Json::GetStr(setters["actor"],actorStr))
    {
        return;
    }
    SetActorID(AutoId(actorStr));
    LoadComponentLoaders(setters,mComponentLoaders);
}

void SpawnActorMapElement::SetActorID(int32_t actorID)
{
    mActorID=actorID;
}

int32_t SpawnActorMapElement::GetActorID()const
{
    return mActorID;
}

ActorCreator::ComponentLoaderMap_t const& SpawnActorMapElement::GetComponentLoaders()const
{
    return mComponentLoaders;
}

void SpawnActorMapElement::LoadComponentLoaders(Json::Value& setters, ActorCreator::ComponentLoaderMap_t& componentLoaders)
{
    Json::Value& components=setters["components"];
    if (!components.isArray())
    {
        return;
    }
    if (components.empty())
    {
        return;
    }
    ComponentLoaderFactory& componentLoaderFactory=ComponentLoaderFactory::Get();
    for( Json::Value::iterator i = components.begin(), e = components.end(); i != e; ++i )
    {
        Json::Value& component=*i;
        std::string compName;
        if (!Json::GetStr( component["name"],compName))
        {
            return;
        }
        Json::Value& compSetters=component["set"];
        int32_t componentId=AutoId(compName);
        std::auto_ptr<ActorCreator::ComponentLoader_t> compLoader=componentLoaderFactory(componentId);
        if(compSetters.isArray()&&!compSetters.empty())
        {
            compLoader->Load(*compSetters.begin());
        }

        componentLoaders.insert(componentId,compLoader);
    }
}

void SpawnActorMapElement::AddComponentLoader(int32_t componentId, std::auto_ptr<ActorCreator::ComponentLoader_t> compLoader)
{
    mComponentLoaders.insert(componentId,compLoader);
}

void SpawnActorMapElement::Save(Json::Value& Element)
{
    Opt<Actor> actor(Scene::Get().GetActor(mSpawnedActorGUID));
    if (!actor.IsValid())
    {
        return;
    }
    MapElement::Save(Element);

    std::string actorName;
    if (IdStorage::Get().GetName(mActorID,actorName))
    {
        Element["actor"]=Json::Value(actorName);
    }
    Json::Value ComponentsArr(Json::arrayValue);

    Opt<IPositionComponent> positionC(actor->Get<IPositionComponent>());
    if (positionC.IsValid())
    {
        Json::Value Component(Json::objectValue);
        positionC->Save(Component);
        ComponentsArr.append(Component);
    }
    Opt<IBorderComponent> borderC(actor->Get<IBorderComponent>());
    if (borderC.IsValid())
    {
        Json::Value Component(Json::objectValue);
        borderC->Save(Component);
        ComponentsArr.append(Component);
    }

    Element["components"]=Json::Value(ComponentsArr);
}

} // namespace map
