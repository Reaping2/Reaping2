#ifndef INCLUDED_PLATFORM_MODEL_VALUE_H
#define INCLUDED_PLATFORM_MODEL_VALUE_H

#include <boost/function.hpp>
#include "stdint.h"
#include <string>
#include "singleton.h"
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

namespace platform {

/*
 * TODO: 
 * hide impl details
 * change semantics ( probably shared-ptr semantics would be better than auto-ptr sem )
 */

class ModelValue
{
public:
    typedef boost::function<void( void )> void_function_t;
    typedef boost::function<void( int32_t )> int_function_t;
    typedef boost::function<void( double )> double_function_t;
    typedef boost::function<void( std::string const& )> string_function_t;
private:
    enum Type
    {
        Mt_Int,
        Mt_Double,
        Mt_String,
        Mt_VoidFunction,
        Mt_IntFunction,
        Mt_DoubleFunction,
        Mt_StringFunction,
        Mt_Multi,
        Mt_None,
    };
    Type mType;
    union Union_t
    {
        int32_t const* i;
        double const* d;
        std::string const* s;
        void* f;
        Union_t( int32_t const& v ): i( &v ) {}
        Union_t( double const& v ): d( &v ) {}
        Union_t( std::string const& v ): s( &v ) {}
        Union_t( void* v ): f( v ) {}
        Union_t(): i( 0 ) {}
    } mValue;
    typedef boost::unordered_map<std::string const, ModelValue*> ModelMap_t;
    ModelMap_t mModels;
    ModelValue* mParent;
    ModelValue( ModelValue const& Other );
    ModelValue& operator=( ModelValue const& Other );
public:
    virtual ~ModelValue();
    ModelValue( int32_t const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( double const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( std::string const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( void_function_t const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( int_function_t const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( double_function_t const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( string_function_t const& ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( std::string const& Name, ModelValue* Parent );
    explicit ModelValue( std::string const& Name );

    /*not supposed to be used below render and ui*/
    ModelValue const& operator[]( char const* Name )const;
    ModelValue const& operator[]( std::string const& Name )const;

    operator int32_t() const;
    operator double() const;
    operator std::string() const;
    void operator()() const;
    void operator()( int32_t Arg ) const;
    void operator()( double Arg ) const;
    void operator()( std::string const& Arg ) const;

    bool IsValid() const;
};

class RootModel : public Singleton<RootModel>, public ModelValue
{
    friend class Singleton<RootModel>;
    RootModel();
};

template<typename BASE, typename FUN>
inline ModelValue::void_function_t VoidFunc( BASE* b, FUN f )
{
    return ModelValue::void_function_t( boost::bind( f, b ) );
}

template<typename BASE, typename FUN>
inline ModelValue::string_function_t StringFunc( BASE* b, FUN f )
{
    return ModelValue::string_function_t( boost::bind( f, b, _1 ) );
}

template<typename BASE, typename FUN>
inline ModelValue::int_function_t IntFunc( BASE* b, FUN f )
{
    return ModelValue::int_function_t( boost::bind( f, b, _1 ) );
}

template<typename BASE, typename FUN>
inline ModelValue::double_function_t DoubleFunc( BASE* b, FUN f )
{
    return ModelValue::double_function_t( boost::bind( f, b, _1 ) );
}

} // namespace platform

#endif//INCLUDED_PLATFORM_MODEL_VALUE_H
