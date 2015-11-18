#include "platform/i_platform.h"
#include "core/component.h"
#include "core/component_factory.h"

const double PRECISION=100.0;

Component::Component()
    : mActor( NULL )
{
}
Component::~Component()
{
}

void Component::SetActor(Actor* Obj)
{
    mActor=Obj;
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

