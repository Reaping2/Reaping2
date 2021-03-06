#ifndef INCLUDED_CORE_REPOSITORY_H
#define INCLUDED_CORE_REPOSITORY_H
#include <boost/ptr_container/ptr_map.hpp>
namespace platform {

template<typename Element_T>
class Repository
{
protected:
    typedef Repository<Element_T> RepoBase;
public:
    typedef boost::ptr_map<int32_t, Element_T> ElementMap_t;
    typedef typename ElementMap_t::const_iterator const_iterator;
    typedef typename ElementMap_t::iterator iterator;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
protected:
    ElementMap_t mElements;
    Element_T const& mDefaultElement;
    Repository( Element_T const& DefaultElement );
    virtual ~Repository() {}
public:
    virtual bool HasElem( int32_t Id )const;
    virtual Element_T const& operator()( int32_t Id ) const;
    virtual Element_T& operator()( int32_t Id );  // lazy load
};

template<typename Element_T>
bool Repository<Element_T>::HasElem( int32_t Id ) const
{
    return mElements.end() != mElements.find( Id );
}

template<typename Element_T>
Element_T& Repository<Element_T>::operator()( int32_t Id )
{
    return const_cast<Element_T&>( ( ( const RepoBase* )this )->operator ()( Id ) );
}

template<typename Element_T>
Element_T const& Repository<Element_T>::operator()( int32_t Id ) const
{
    typename ElementMap_t::const_iterator i = mElements.find( Id );
    return i == mElements.end() ? mDefaultElement : *( i->second );
}

template<typename Element_T>
typename Repository<Element_T>::iterator Repository<Element_T>::begin()
{
    return mElements.begin();
}

template<typename Element_T>
typename Repository<Element_T>::iterator Repository<Element_T>::end()
{
    return mElements.end();
}

template<typename Element_T>
typename Repository<Element_T>::const_iterator Repository<Element_T>::begin() const
{
    return mElements.begin();
}

template<typename Element_T>
typename Repository<Element_T>::const_iterator Repository<Element_T>::end() const
{
    return mElements.end();
}

// template<typename Element_T>
// Element_T& Repository<Element_T>::operator()( int32_t Id )
// {
//     return const_cast<Element_T&>(((const RepoBase*)this)->operator()(id));
// }

template<typename Element_T>
Repository<Element_T>::Repository( Element_T const& DefaultElement )
    : mDefaultElement( DefaultElement )
{
}

} // namespace platform

#endif//INCLUDED_CORE_REPOSITORY_H
