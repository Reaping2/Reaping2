#ifndef INCLUDED_CORE_ACTOR_FACTORY_H
#define INCLUDED_CORE_ACTOR_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include <boost/ptr_container/ptr_map.hpp>
#include "core/component.h"
#include "core/component_factory.h"
#include "core/component_loader_factory.h"
#include "core/property_loader.h"
#include "core/actor.h"
#include "platform/i_platform.h"

class ActorCreator
{
public:
    ActorCreator();
    typedef ComponentLoader<Component> ComponentLoader_t;
    typedef boost::ptr_map<int32_t, ComponentLoader_t > ComponentLoaderMap_t;
    ComponentLoaderMap_t mComponentLoaders;
    void SetId( int32_t id );
    int32_t GetId();
    void AddComponent( int32_t componentId, Json::Value const& setters );
    std::auto_ptr<Actor> Create()const;
private:
    ComponentFactory& mComponentFactory;
    ComponentLoaderFactory& mComponentLoaderFactory;
    int32_t mId;

};

class ActorFactory : public platform::Factory<Actor>, public platform::Singleton<ActorFactory>
{
    typedef boost::ptr_map<int32_t, ActorCreator> ActorCreatorMap_t;
    ActorCreatorMap_t mActorCreators;
    friend class platform::Singleton<ActorFactory>;
    ActorFactory();

    void Init();
protected:
    std::auto_ptr<Actor> CreateActor( int32_t Id );
};



#endif//INCLUDED_CORE_ACTOR_FACTORY_H
