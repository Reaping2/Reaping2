#include "platform/i_platform.h"
#include "core/actor_factory.h"
#include "core/property_loader.h"
#include "core/i_renderable_component.h"

ActorFactory::ActorFactory()
{
    Init();
    //every created actor is handled the same way.
    //actorCreators create actors from json data files.
    Bind( AutoId( "default_actor" ),boost::bind(&ActorFactory::CreateActor,this,_1) );
    SetDefault( AutoId( "default_actor" ) );
}

std::auto_ptr<Actor> ActorFactory::CreateActor( int32_t Id )
{
    ActorCreatorMap_t::iterator i = mActorCreators.find(Id);
    return (i!=mActorCreators.end())?i->second->Create():std::auto_ptr<Actor>( new Actor(Id) );
}


bool ActorFactory::AddActorCreatorFromOneDesc( Json::Value& ActorsDesc, ActorCreatorMap_t& actorCreators )
{
    std::auto_ptr<ActorCreator> actorCreator(new ActorCreator());
    std::string nameStr;
    if( !Json::GetStr( ActorsDesc["name"], nameStr))
    {
        return false;
    }
    actorCreator->SetId(AutoId(nameStr));
    Json::Value& components=ActorsDesc["components"];
    if (!components.isArray())
    {
        return false;
    }
    if (components.empty())
    {
        return true;
    }
    for( Json::Value::iterator i = components.begin(), e = components.end(); i != e; ++i )
    {
        Json::Value& component=*i;
        std::string compName;
        if (!Json::GetStr( component["name"],compName))
        {
            return false;
        }
        Json::Value& setters=component["set"];
        actorCreator->AddComponent(AutoId(compName),setters);
    }
    actorCreators.insert(actorCreator->GetId(), actorCreator);
    return true;
}

void ActorFactory::Init()
{
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, "actors" );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".json" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Root.isArray() )
        {
            continue;
        }
        for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
        {
            Json::Value& ActorsDesc = *i;
            if( !AddActorCreatorFromOneDesc( ActorsDesc, mActorCreators ) )
            {
                return;
            }
        }
    }
}

void ActorCreator::SetId(int32_t id)
{
    mId=id;
}

int32_t ActorCreator::GetId()
{
    return mId;
}

void ActorCreator::AddComponent(int32_t componentId, Json::Value& setters)
{
    std::auto_ptr<ComponentLoader_t> compLoader=mComponentLoaderFactory(componentId);
    if(setters.isArray()&&!setters.empty())
    {
        compLoader->Load(*setters.begin());
    }

    mComponentLoaders.insert(componentId,compLoader);
}

std::auto_ptr<Actor> ActorCreator::Create()const
{
    std::auto_ptr<Actor> actor(new Actor(mId));
    for(ComponentLoaderMap_t::const_iterator i=mComponentLoaders.begin(), e=mComponentLoaders.end();i!=e;++i)
    {
        actor->AddComponent(i->second->FillProperties(mComponentFactory(i->first)));
    }
    //TODO: guid will come from outside zorder will be done by ordering at renderer
    Opt<IRenderableComponent> renderableC = actor->Get<IRenderableComponent>();
    if (renderableC.IsValid())
    {
        renderableC->SetZOrder(actor->GetGUID());
    }
    return actor;
}

ActorCreator::ActorCreator()
    : mComponentFactory(ComponentFactory::Get())
    , mComponentLoaderFactory(ComponentLoaderFactory::Get())
{

}

