#include "core/map/spawn_actor_map_element.h"
#include "../scene.h"
#include "../i_position_component.h"
#include "../i_border_component.h"
#include "../position_component.h"

namespace map {

int32_t SpawnActorMapElement::SpawnNodeId()
{
    static int32_t id = AutoId( "spawn" );
    return id;
}


SpawnActorMapElement& SpawnActorMapElement::operator=( SpawnActorMapElement const& other )
{
    MapElement::operator=( other );
    BaseInput::operator=( other );
    mActorID = other.mActorID;
    for (auto&& compload : other.mComponentLoaders)
    {
        int32_t i = compload.first;
        mComponentLoaders.insert( i, static_cast<ActorCreator::ComponentLoader_t*>(compload.second->clone()) );
    }
    mRemoveWhenUsed = other.mRemoveWhenUsed;
    return *this;

}

SpawnActorMapElement::SpawnActorMapElement( int32_t Id )
    : MapElement( Id )
    , BaseInput()
    , mActorID( -1 )
    , mRemoveWhenUsed( true )
{
    AddInputNodeId( SpawnNodeId() );
}


SpawnActorMapElement::SpawnActorMapElement( SpawnActorMapElement const& other )
{
    *this = other;
}

void SpawnActorMapElement::Load( Json::Value const& setters )
{
    MapElement::Load( setters );

    std::string actorStr;
    if ( !Json::GetStr( setters["actor"], actorStr ) )
    {
        return;
    }
    SetActorID( AutoId( actorStr ) );
    LoadComponentLoaders( setters, mComponentLoaders );
    int32_t removeWhenUsed;
    if (Json::GetInt( setters["remove_when_used"], removeWhenUsed ))
    {
        mRemoveWhenUsed = removeWhenUsed != 0;
    }
}

void SpawnActorMapElement::SetActorID( int32_t actorID )
{
    mActorID = actorID;
}

int32_t SpawnActorMapElement::GetActorID()const
{
    return mActorID;
}

ActorCreator::ComponentLoaderMap_t const& SpawnActorMapElement::GetComponentLoaders()const
{
    return mComponentLoaders;
}

void SpawnActorMapElement::SetRemoveWhenUsed( bool removeWhenUsed )
{
    mRemoveWhenUsed = removeWhenUsed;
}

bool SpawnActorMapElement::IsRemoveWhenUsed() const
{
    return mRemoveWhenUsed;
}

void SpawnActorMapElement::LoadComponentLoaders( Json::Value const& setters, ActorCreator::ComponentLoaderMap_t& componentLoaders )
{
    auto const& components = setters["components"];
    if ( !components.isArray() )
    {
        return;
    }
    if ( components.empty() )
    {
        return;
    }
    auto& componentLoaderFactory(ComponentLoaderFactory::Get());
    for( auto&& component: components )
    {
        std::string compName;
        if ( !Json::GetStr( component["name"], compName ) )
        {
            return;
        }
        auto const& compSetters = component["set"];
        int32_t componentId = AutoId( compName );
        std::auto_ptr<PropertyLoaderBase<Component> > compLoader = componentLoaderFactory( componentId );
        if( compSetters.isArray() && !compSetters.empty() )
        {
            compLoader->Load( *compSetters.begin() );
        }

        componentLoaders.insert( componentId, static_cast<ActorCreator::ComponentLoader_t*>( compLoader.release() ) );
    }
}

void SpawnActorMapElement::AddComponentLoader( int32_t componentId, std::auto_ptr<PropertyLoaderBase<Component> > compLoader )
{
    mComponentLoaders.insert( componentId, static_cast<ActorCreator::ComponentLoader_t*>( compLoader.release() ) );
}

Opt<PropertyLoaderBase<Component>> SpawnActorMapElement::GetComponentLoader( int32_t componentId )
{
    Opt<PropertyLoaderBase<Component>> r;
    auto found = mComponentLoaders.find( componentId );
    if (found != mComponentLoaders.end())
    {
        r = found->second;
    }
    return r;
}

void SpawnActorMapElement::Save( Json::Value& Element )
{
    Opt<Actor> actor( Scene::Get().GetActor( mSpawnedActorGUID ) );
    if ( !actor.IsValid() )
    {
        return;
    }
    MapElement::Save( Element );

    std::string actorName;
    if ( IdStorage::Get().GetName( mActorID, actorName ) )
    {
        Element["actor"] = Json::Value( actorName );
    }
    Json::Value ComponentsArr( Json::arrayValue );

    Opt<IPositionComponent> positionC( actor->Get<IPositionComponent>() );
    if ( positionC.IsValid() )
    {
        Json::Value Component( Json::objectValue );
        positionC->Save( Component );
        ComponentsArr.append( Component );
    }
    Opt<IBorderComponent> borderC( actor->Get<IBorderComponent>() );
    if ( borderC.IsValid() )
    {
        Json::Value Component( Json::objectValue );
        borderC->Save( Component );
        ComponentsArr.append( Component );
    }

    Element["components"] = Json::Value( ComponentsArr );
}

} // namespace map
