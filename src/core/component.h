#ifndef INCLUDED_CORE_COMPONENT_H
#define INCLUDED_CORE_COMPONENT_H


#define DEFINE_COMPONENT_BASE( ComponentType ) \
    static int GetType_static() \
    { \
        static int const typ = AutoId( #ComponentType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return ComponentType::GetType_static(); \
    } \


class Component 
{
public:
	DEFINE_COMPONENT_BASE(Component)
	virtual ~Component();

protected:
	Component();
};

class ComponentHolder
{
protected:
	typedef boost::ptr_map<int32_t, Component> ComponentList_t;
	ComponentRepo& mComponentFactory;
	ComponentList_t mComponents;

public:
	ComponentHolder();
	void AddComponent( std::auto_ptr<Component> Comp  );
   
	template<typename Component_t>
	Opt<Component_t> Get() const;
	template<typename Component_t>
	Opt<Component_t> Get();
	virtual ~ComponentHolder();
};
template<typename Component_t>
Opt<Component_t> ComponentHolder::Get() const
{
	ComponentList_t::const_iterator i = mComponents.find( Component_t::GetType_static() );
	BOOST_ASSERT(i != mComponents.end()); // if this one is not true the we a screwed
	return (Opt<Component_t>(static_cast<Component_t*>(const_cast<Component*>(i->second)))); 
}

template<typename Component_t>
Opt<Component_t> ComponentHolder::Get()
{
	return ( ( const ComponentHolder* )this )->Get<Component_t>();
}

class DefaultComponent : public Component
{
public:
	DEFINE_COMPONENT_BASE(DefaultComponent)
	DefaultComponent();
	friend class Factory<Component>;
};
#endif//INCLUDED_CORE_COMPONENT_H
