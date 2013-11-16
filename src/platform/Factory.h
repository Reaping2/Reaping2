#ifndef INCLUDED_PLATFORM_FACTORY_H
#define INCLUDED_PLATFORM_FACTORY_H
template<typename Return_T, typename Element_T>
class Factory : public Singleton< Factory<Return_T, Element_T> >
{
	friend class Singleton< Factory<Return_T, Element_T> >;
public:
	typedef typename Factory<Return_T,Element_T> FactoryBase;
	typedef boost::function<Return_T*(int32_t, Element_T&)> Functor_t;
	typedef std::map<int32_t, Functor_t> ElementMap_t;

	template<typename Elem_T>
	void Bind( int32_t Id )
	{
		mElements[Id]=boost::bind(&Create<Elem_T>,_1,_2);
	}
	void SetDefault( int32_t Id );

	Return_T * operator()(int32_t Id, Element_T& actor) const;
	Return_T * operator()(int32_t Id, Element_T& actor);	// lazy load
protected:
	Functor_t mDefaultElement;
	ElementMap_t mElements;
	template<typename Elem_T>
	static Return_T * Create(int32_t Id, Element_T& actor)
	{
		return new Elem_T(Id,actor);
	}


};
template<typename Return_T, typename Element_T>
void Factory<Return_T,Element_T>::SetDefault( int32_t Id )
{
	ElementMap_t::iterator it = mElements.find(Id);
	if(it!=mElements.end())
		mDefaultElement=it->second;
}
template<typename Return_T, typename Element_T>
Return_T * Factory<Return_T,Element_T>::operator()( int32_t Id, Element_T& actor )
{
	return ((const FactoryBase*)this)->operator ()(Id,actor);
}

template<typename Return_T, typename Element_T>
Return_T * Factory<Return_T,Element_T>::operator()( int32_t Id, Element_T& actor ) const
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	return (i==mElements.end()?mDefaultElement(Id,actor):(i->second)(Id,actor));
}
#endif//INCLUDED_PLATFORM_FACTORY_H