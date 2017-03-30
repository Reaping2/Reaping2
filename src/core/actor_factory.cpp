#include "platform/i_platform.h"
#include "core/actor_factory.h"
#include "core/property_loader.h"
#include "core/i_renderable_component.h"
#include "component.h"
#include "platform/filesystem_utils.h"

ActorFactory::ActorFactory()
{
    Init();
    //every created actor is handled the same way.
    //actorCreators create actors from json data files.
    Bind( AutoId( "default_actor" ), boost::bind( &ActorFactory::CreateActor, this, _1 ) );
    SetDefault( AutoId( "default_actor" ) );
}

std::auto_ptr<Actor> ActorFactory::CreateActor( int32_t Id )
{
    ActorCreatorMap_t::iterator i = mActorCreators.find( Id );
    return ( i != mActorCreators.end() ) ? i->second->Create() : std::auto_ptr<Actor>( new Actor( Id ) );
}


void ActorFactory::Init()
{
    fs_utils::for_each( "actors", ".actor", [&]( Json::Value const& desc )
    {
        std::string nameStr;
        if (!Json::GetStr( desc["name"], nameStr ))
        {
            return;
        }
        auto const& components = desc["components"];
        if (!components.isArray()|| components.empty())
        {
            return;
        }
        std::auto_ptr<ActorCreator> actorCreator( new ActorCreator() );
        actorCreator->SetId( AutoId( nameStr ) );
        for (auto&& component: components)
        {
            std::string compName;
            if (!Json::GetStr( component["name"], compName ))
            {
                return;
            }
            auto const& setters = component["set"];
            actorCreator->AddComponent( AutoId( compName ), setters );
        }
        int32_t actorCreatorId = actorCreator->GetId();
        mActorCreators.insert( actorCreatorId, actorCreator );
    } );
}

void ActorCreator::SetId( int32_t id )
{
    mId = id;
}

int32_t ActorCreator::GetId()
{
    return mId;
}

void ActorCreator::AddComponent( int32_t componentId, Json::Value const& setters )
{
    std::auto_ptr<PropertyLoaderBase<Component> > compLoader = mComponentLoaderFactory( componentId );
    if( setters.isArray() && !setters.empty() )
    {
        compLoader->Load( *setters.begin() );
    }
    else if (setters.isObject())
    {
        compLoader->Load( setters );
    }

    mComponentLoaders.insert( componentId, static_cast<ComponentLoader_t*>( compLoader.release() ) );
}

std::auto_ptr<Actor> ActorCreator::Create()const
{
    std::auto_ptr<Actor> actor( new Actor( mId ) );
    for( ComponentLoaderMap_t::const_iterator i = mComponentLoaders.begin(), e = mComponentLoaders.end(); i != e; ++i )
    {
        //        actor->AddComponent(i->second->FillProperties(mComponentFactory(i->first)));
        actor->AddComponent( mComponentFactory( i->first ) );
        i->second->FillProperties( *actor.get() );
    }
    //TODO: guid will come from outside zorder will be done by ordering at renderer
    Opt<IRenderableComponent> renderableC = actor->Get<IRenderableComponent>();
    if ( renderableC.IsValid() )
    {
        renderableC->SetZOrder( actor->GetGUID() );
    }
    return actor;
}

ActorCreator::ActorCreator()
    : mComponentFactory( ComponentFactory::Get() )
    , mComponentLoaderFactory( ComponentLoaderFactory::Get() )
    , mId( -1 )
{

}

