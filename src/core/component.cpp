#include "i_core.h"

Component::Component()
{
}
Component::~Component()
{
}

void ComponentHolder::AddComponent( std::auto_ptr<Component> Comp )
{
    ComponentList_t::iterator i = mComponents.find( Comp->GetType_interface() );
    if( i == mComponents.end() )
    {
        mComponents.insert( Comp->GetType_interface(), Comp );
    }
}

ComponentHolder::ComponentHolder()
    : mComponentFactory( ComponentRepo::Get() )
{
}

ComponentHolder::~ComponentHolder()
{
}


DefaultComponent::DefaultComponent()
    : Component()
{
}

