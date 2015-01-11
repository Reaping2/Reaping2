#include "platform/i_platform.h"
#include "core/component.h"
#include "core/component_factory.h"

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

void ComponentHolder::AddComponent( std::auto_ptr<Component> Comp )
{
    ComponentList_t::iterator i = mComponents.find( Comp->GetType() );
    if( i == mComponents.end() )
    {
        mComponents.insert( Comp->GetType(), Comp );
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

