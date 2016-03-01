#include "platform/i_platform.h"
#include "core/component.h"
#include "core/component_factory.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

const double PRECISION=1000.0;

Component::Component()
    : mActorGUID( -1 )
{
}
Component::~Component()
{
}

void Component::SetActorGUID(int32_t actorGUID)
{
    mActorGUID=actorGUID;
}

void Component::SetId(int32_t id)
{
    mId = id;
}

int32_t Component::GetId() const
{
    return mId;
}

void Component::Save(Json::Value& component)
{
    std::string componentName;
    if (IdStorage::Get().GetName(mId,componentName))
    {
        component["name"]=Json::Value(componentName);
    }
}

int Component::GetType() const
{
    return 0;
}

void ComponentHolder::AddComponent( std::auto_ptr<Component> Comp )
{
    ComponentList_t::iterator i = mComponents.find( Comp->GetType() );
    if( i == mComponents.end() )
    {
        int type=Comp->GetType();
        mComponents.insert( type, Comp );
    }
}

ComponentHolder::ComponentHolder()
    : mComponentFactory( ComponentFactory::Get() )
{
}

ComponentHolder::~ComponentHolder()
{
}


DefaultComponent::DefaultComponent()
    : Component()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT(DefaultComponent, DefaultComponent);
