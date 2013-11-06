#ifndef INCLUDED_CORE_REPOSITORY_H
#define INCLUDED_CORE_REPOSITORY_H
template<typename Self_T, typename Element_T>
class Repository: public Singleton<Self_T>
{
protected:
	typedef boost::ptr_map<int32_t, Element_T> ElementMap_t;
	ElementMap_t mElements;
	Element_T& mDefaultElement;
	Repository(Element_T& DefaultElement);
public:
	Element_T const& GetById(int32_t Id) const;
	Element_T const& GetByName(std::string const& What) const;
};

template<typename Self_T, typename Element_T>
Element_T const& Repository<Self_T,Element_T>::GetById(int32_t Id) const
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	return i==mElements.end()?mDefaultElement:*(i->second);
}
template<typename Self_T, typename Element_T>
Element_T const& Repository<Self_T,Element_T>::GetByName(std::string const& What) const
{
	return GetById(IdStorage::Get().GetId(What));
}

template<typename Self_T, typename Element_T>
Repository<Self_T,Element_T>::Repository(Element_T& DefaultElement)
	:mDefaultElement(DefaultElement)
{
}
#endif INCLUDED_CORE_REPOSITORY_H