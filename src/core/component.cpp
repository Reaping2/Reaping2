#include "i_core.h"

Component::Component( int32_t Id )
	: mId( Id )
	, mComponentFactory( ComponentRepo::Get() )
{
}

void Component::AddComponent( int32_t Id )
{
	ComponentList_t::iterator i = mComponents.find( Id );
    if( i == mComponents.end() )
    {
        mComponents.insert( Id, mComponentFactory( Id ) );
    }
}

bool Component::HasComponent( int32_t Id )
{
	return mComponents.find( Id ) != mComponents.end();
}

DefaultComponent::DefaultComponent( int32_t Id )
	: Component( Id )
{
}

ComponentDependent::ComponentDependent()
{
}

void ComponentDependent::AddDependency( int32_t Id )
{
	mDependencies.push_back( Id );
}
	
void ComponentDependent::InitDependencies( Component& Comp )
{
	for( DependencyList_t::iterator i=mDependencies.begin(), e=mDependencies.end();i!=e;++i)
	{
		int32_t dep=*i;
		if (Comp.HasComponent(dep))
		{
			mComponentDependents.insert(std::pair<int32_t, Component&>(dep,Comp.GetComponent<Component>(dep)));
		}
	}
}

