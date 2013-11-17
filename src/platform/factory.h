#ifndef INCLUDED_PLATFORM_FACTORY_H
#define INCLUDED_PLATFORM_FACTORY_H
template<typename Return_T>
class Factory : public Singleton< Factory<Return_T> >
{
	friend class Singleton< Factory<Return_T> >;
public:
	typedef typename Factory<Return_T> FactoryBase;
	typedef boost::function<Return_T*(int32_t)> Functor_t;
	typedef std::map<int32_t, Functor_t> ElementMap_t;

	template<typename Elem_T>
	void Bind( int32_t Id )
	{
		BOOST_STATIC_ASSERT_MSG(
		(boost::is_base_of<Return_T, Elem_T>::value),
		"Elem_T must be a descendant of Return_T"
		);
		mElements[Id]=boost::bind(&Create<Elem_T>,_1);
	}
	void SetDefault( int32_t Id );

	Return_T * operator()(int32_t Id) const;
	Return_T * operator()(int32_t Id);	// lazy load
protected:
	Functor_t mDefaultElement;
	ElementMap_t mElements;
	template<typename Elem_T>
	static Return_T * Create(int32_t Id)
	{
		return new Elem_T(Id);
	}


};
template<typename Return_T>
void Factory<Return_T>::SetDefault( int32_t Id )
{
	ElementMap_t::iterator it = mElements.find(Id);
	if(it!=mElements.end())
		mDefaultElement=it->second;
}
template<typename Return_T>
Return_T * Factory<Return_T>::operator()( int32_t Id )
{
	return ((const FactoryBase*)this)->operator ()(Id);
}

template<typename Return_T>
Return_T * Factory<Return_T>::operator()(int32_t Id) const
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	return (i==mElements.end()?mDefaultElement(Id):(i->second)(Id));
}
#endif//INCLUDED_PLATFORM_FACTORY_H