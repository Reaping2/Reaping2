#ifndef INCLUDED_PLATFORM_MODEL_VALUE_H
#define INCLUDED_PLATFORM_MODEL_VALUE_H

#include <boost/function.hpp>
#include "rstdint.h"
#include <string>
#include "singleton.h"
#include <boost/lambda/bind.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ref.hpp>

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
    typedef boost::function<int32_t()> get_int_t;
    typedef boost::function<double()> get_double_t;
    typedef boost::function<std::string()> get_string_t;

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
    void* mValue;
    boost::function< int32_t() > mGetInt;
    typedef boost::unordered_map<std::string const, ModelValue*> ModelMap_t;
    ModelMap_t mModels;
    ModelValue* mParent;
    ModelValue( ModelValue const& Other );
    ModelValue& operator=( ModelValue const& Other );
public:
    virtual ~ModelValue();
    ModelValue( get_int_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( get_double_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( get_string_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( void_function_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( int_function_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( double_function_t ModelFor, std::string const& Name, ModelValue* Parent );
    ModelValue( string_function_t ModelFor, std::string const& Name, ModelValue* Parent );
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
    return ModelValue::void_function_t( boost::lambda::bind( f, b ) );
}

#define CALL_HELPER_FUNC( FnName, fn ) \
template<typename BASE, typename FUN> \
inline ModelValue::fn FnName( BASE* b, FUN f ) \
{ \
    return ModelValue::fn( boost::lambda::bind( f, b, boost::lambda::_1 ) ); \
}
#define GET_HELPER_FUNC( FnName, fn ) \
template<typename BASE, typename FUN> \
inline ModelValue::fn FnName( BASE* b, FUN f ) \
{ \
    return ModelValue::fn( boost::lambda::bind( f, b ) ); \
}

CALL_HELPER_FUNC( StringFunc, string_function_t )
CALL_HELPER_FUNC( IntFunc, int_function_t )
CALL_HELPER_FUNC( DoubleFunc, double_function_t )
GET_HELPER_FUNC( GetStringFunc, get_string_t )
GET_HELPER_FUNC( GetIntFunc, get_int_t )
GET_HELPER_FUNC( GetDoubleFunc, get_double_t )

#undef CALL_HELPER_FUNC
#undef GET_HELPER_FUNC

template<typename T>
T RefToHelper( T const& t )
{
    return t;
}

template<typename T>
inline boost::function<T()> RefTo( T const& t )
{
    return boost::lambda::bind( &RefToHelper<T>, boost::cref( t ) );
}

} // namespace platform

#endif//INCLUDED_PLATFORM_MODEL_VALUE_H
