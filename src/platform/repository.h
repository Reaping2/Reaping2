#ifndef INCLUDED_CORE_REPOSITORY_H
#define INCLUDED_CORE_REPOSITORY_H

namespace platform {

/* TODO: default elem should not be a singleton. it's better to provide an overrideable GetDefault() method */

template<typename Element_T>
class Repository
{
protected:
    typedef Repository<Element_T> RepoBase;
    typedef boost::ptr_map<int32_t, Element_T> ElementMap_t;
    ElementMap_t mElements;
    Element_T const& mDefaultElement;
    Repository( Element_T const& DefaultElement );
    virtual ~Repository() {}
    virtual Element_T const& GetDefaultElement() const;
public:
    virtual bool HasElem( int32_t Id )const;
    virtual Element_T const& operator()( int32_t Id ) const;
    virtual Element_T const& operator()( int32_t Id );  // lazy load
};

template<typename Element_T>
Element_T const& Repository<Element_T>::GetDefaultElement() const
{
#warning "mDefaultElement is obsolete and will be removed soon. Override this method instead."
    return mDefaultElement;
}

template<typename Element_T>
bool Repository<Element_T>::HasElem( int32_t Id ) const
{
    return mElements.end() != mElements.find( Id );
}

template<typename Element_T>
Element_T const& Repository<Element_T>::operator()( int32_t Id )
{
    return ( ( const RepoBase* )this )->operator ()( Id );
}

template<typename Element_T>
Element_T const& Repository<Element_T>::operator()( int32_t Id ) const
{
    typename ElementMap_t::const_iterator i = mElements.find( Id );
    return i == mElements.end() ? GetDefaultElement() : *( i->second );
}

template<typename Element_T>
Repository<Element_T>::Repository( Element_T const& DefaultElement )
    : mDefaultElement( DefaultElement )
{
}

} // namespace platform

#endif//INCLUDED_CORE_REPOSITORY_H
