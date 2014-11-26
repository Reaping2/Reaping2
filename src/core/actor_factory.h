#ifndef INCLUDED_CORE_ACTOR_FACTORY_H
#define INCLUDED_CORE_ACTOR_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include <boost/ptr_container/ptr_map.hpp>
#include "core/component.h"
#include "core/component_factory.h"
#include "i_core.h"
class Actor;

class ActorCreator
{
public:
    ActorCreator();
    typedef std::list<int32_t> ComponentList_t;
    ComponentList_t mComponents;
    void SetId(int32_t id);
    int32_t GetId();
    void AddComponent(int32_t componentId);
    std::auto_ptr<Actor> Create()const; 
private:
    ComponentFactory& mComponentFactory;
    int32_t mId;

};

class ActorFactory : public platform::Factory<Actor>, public platform::Singleton<ActorFactory>
{
    typedef boost::ptr_map<int32_t,ActorCreator> ActorCreatorMap_t;
    ActorCreatorMap_t mActorCreators;
    friend class platform::Singleton<ActorFactory>;
    ActorFactory();

    void Init();
    bool AddActorCreatorsFromOneDesc( Json::Value& TexDesc, ActorCreatorMap_t& actorCreators );
protected:
    std::auto_ptr<Actor> CreateActor( int32_t Id );
};



#endif//INCLUDED_CORE_ACTOR_FACTORY_H