#include "i_core.h"

Component::Component()
{
}
Component::~Component()
{
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

