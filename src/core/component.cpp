#include "i_core.h"
#include "core/component.h"

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

