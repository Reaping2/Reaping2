#ifndef INCLUDED_CORE_COMPONENT_H
#define INCLUDED_CORE_COMPONENT_H

class Component 
{
public:
	virtual ~Component() {};

	typedef boost::ptr_map<int32_t, Component> ComponentList_t;
	ComponentRepo& mComponentFactory;
	ComponentList_t mComponents;

	void AddComponent( int32_t Id );
    bool HasComponent( int32_t Id );
	template<typename Component_t>
	const Component_t& GetComponent( int32_t Id ) const
	{
		ComponentList_t::const_iterator i = mComponents.find( Id );
		BOOST_ASSERT(i != mComponents.end()); // if this one is not true the we a screwed
		return static_cast<const Component_t&>(*i->second); 
	}
	template<typename Component_t>
	Component_t& GetComponent( int32_t Id )
	{
		return const_cast<Component_t&>(( ( const Component* )this )->GetComponent<Component_t>( Id ));
	}
protected:
	int32_t mId;
	Component( int32_t Id );
};

class DefaultComponent : public Component
{
public:
	DefaultComponent( int32_t Id );
	friend class Factory<Component>;
};
//TODO: this class is experimental, not designed properly sry for copy-paste
class ComponentDependent
{
public:
	virtual ~ComponentDependent() {};
protected:
	typedef std::list<int32_t> DependencyList_t;
	DependencyList_t mDependencies;
	typedef std::map<int32_t, Component& > ComponentDependentList_t;
	ComponentDependentList_t mComponentDependents;

	void AddDependency( int32_t Id );
	void InitDependencies( Component& Comp );

	template<typename Component_t>
	Component_t& GetDependentComponent( int32_t Id )
	{
		ComponentDependentList_t::iterator i = mComponentDependents.find( Id );
		BOOST_ASSERT(i != mComponentDependents.end()); // if this one is not true the we a screwed
		return dynamic_cast<Component_t&>(i->second); 
	}

	ComponentDependent();
	friend class Factory<Component>;
};
#endif//INCLUDED_CORE_COMPONENT_H
