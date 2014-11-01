#ifndef INCLUDED_PLATFORM_FACTORY_H
#define INCLUDED_PLATFORM_FACTORY_H

#include <memory>
#include "stdint.h"
#include <boost/function.hpp>
#include <map>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>

namespace platform {

template<typename Return_T>
class Factory
{
public:
    virtual ~Factory();
    virtual std::auto_ptr<Return_T> operator()( int32_t Id ) const;
    virtual std::auto_ptr<Return_T> operator()( int32_t Id ); // lazy load
protected:
    typedef Factory<Return_T> FactoryBase;
    typedef boost::function<std::auto_ptr<Return_T>( int32_t )> Functor_t;
    typedef std::map<int32_t, Functor_t> ElementMap_t;
    template<typename Elem_T>
    void Bind( int32_t Id );
    void Bind( int32_t Id, Functor_t Functor );
    void SetDefault( int32_t Id );

    Functor_t mDefaultElement;
    ElementMap_t mElements;
    template<typename Elem_T>
    static std::auto_ptr<Return_T> Create( int32_t Id );
};

template<typename Return_T>
Factory<Return_T>::~Factory()
{
}

template<typename Return_T>
template<typename Elem_T>
std::auto_ptr<Return_T> Factory<Return_T>::Create( int32_t Id )
{
    return std::auto_ptr<Return_T>( new Elem_T( Id ) );
}

template<typename Return_T>
template<typename Elem_T>
void Factory<Return_T>::Bind( int32_t Id )
{
    BOOST_STATIC_ASSERT_MSG(
        ( boost::is_base_of<Return_T, Elem_T>::value ),
        "Elem_T must be a descendant of Return_T"
    );
    mElements[Id] = boost::bind( &Create<Elem_T>, _1 );
}
 
template<typename Return_T>
void Factory<Return_T>::Bind( int32_t Id, Functor_t Functor )
{
    mElements[Id] = Functor;
}

template<typename Return_T>
void Factory<Return_T>::SetDefault( int32_t Id )
{
    typename ElementMap_t::iterator it = mElements.find( Id );
    if( it != mElements.end() )
    {
        mDefaultElement = it->second;
    }
}

template<typename Return_T>
std::auto_ptr<Return_T> Factory<Return_T>::operator()( int32_t Id )
{
    return ( ( const FactoryBase* )this )->operator ()( Id );
}

template<typename Return_T>
std::auto_ptr<Return_T> Factory<Return_T>::operator()( int32_t Id ) const
{
    typename ElementMap_t::const_iterator i = mElements.find( Id );
    return ( i == mElements.end() ? mDefaultElement( Id ) : ( i->second )( Id ) );
}

} // namespace platform

#endif//INCLUDED_PLATFORM_FACTORY_H
