#include "model_value.h"

#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace platform {

#ifdef ENABLE_MODEL_ASSERTS
#define OBSERVABLE_ASSERT(x) BOOST_ASSERT(x)
#else
#define OBSERVABLE_ASSERT(x) (void(0))
#endif

ModelValue::ModelValue( std::string const& Name )
    : mType( Mt_None )
    , mParent( NULL )
{

}

bool ModelValue::IsValid() const
{
    return mType != Mt_None;
}

#define MODELVALUE_CTOR( function_type, mt_type ) \
ModelValue::ModelValue( function_type ModelFor, std::string const& Name, ModelValue* Parent ) \
    : mType( mt_type ) \
    , mValue( (void*) new function_type( ModelFor ) ) \
    , mParent( Parent ) \
{ \
    if( !mParent ) \
    { \
        return; \
    } \
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) ); \
    OBSERVABLE_ASSERT( Inserted.second ); \
}

#define TYPES \
    GET_TYPES \
    CALL_TYPES

#define GET_TYPES \
USE_TYPES( ModelValue::get_int_t, Mt_Int, int32_t ) \
USE_TYPES( ModelValue::get_double_t, Mt_Double, double ) \
USE_TYPES( ModelValue::get_string_t, Mt_String, std::string )

#define CALL_TYPES \
USE_TYPES( ModelValue::void_function_t, Mt_VoidFunction, void ) \
USE_TYPES( ModelValue::int_function_t, Mt_IntFunction, int32_t ) \
USE_TYPES( ModelValue::double_function_t, Mt_DoubleFunction, double ) \
USE_TYPES( ModelValue::string_function_t, Mt_StringFunction, std::string )

#define USE_TYPES( fun, mt, t ) \
    MODELVALUE_CTOR( fun, mt )

TYPES;

#undef USE_TYPES

ModelValue::ModelValue( std::string const& Name, ModelValue* Parent )
    : mType( Mt_Multi )
    , mValue( NULL )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::~ModelValue()
{
    OBSERVABLE_ASSERT( mModels.empty() );
    for( ModelMap_t::iterator i = mModels.begin(), e = mModels.end(); i != e; ++i )
    {
        i->second->mParent = NULL;
    }
    if( mParent )
    {
        ModelMap_t::iterator i = mParent->mModels.begin(), e = mParent->mModels.end();
        while( i != e && i->second != this )
        {
            ++i;
        }
        OBSERVABLE_ASSERT( i != e );
        mParent->mModels.erase( i );
    }
    switch( mType )
    {
#define USE_TYPES( fn, mt, t ) \
        case mt: delete ( fn* ) mValue; break;
TYPES
#undef USE_TYPES
        default:
            break;
    }
}

ModelValue const& ModelValue::operator[]( std::string const& Name ) const
{
    ModelMap_t::const_iterator it = mModels.find( Name );
    if( it == mModels.end() )
    {
        typedef std::vector<std::string> Fields_t;
        Fields_t Fields;
        boost::split( Fields, Name, boost::is_any_of( "." ) );
        Fields_t::const_iterator fi = Fields.begin(), fe = Fields.end();
        ModelMap_t const* Models = &mModels;
        while( fi != fe )
        {
            it = Models->find( *fi++ );
            if( it == Models->end() )
            {
                static ModelValue const DefaultModelValue( "default" );
                return DefaultModelValue;
            }
            Models = &it->second->mModels;
        }
    }
    return *( it->second );
}

ModelValue const& ModelValue::operator[]( char const* Name ) const
{
    return operator[]( std::string( Name ) );
}

ModelValue::operator int32_t() const
{
    if( mType == Mt_Int )
    {
        return ((get_int_t*)mValue)->operator()();
    }
    OBSERVABLE_ASSERT( false );
    return 0;
}

ModelValue::operator double() const
{
    if( mType == Mt_Double )
    {
        return ((get_double_t*)mValue)->operator ()();
    }
    OBSERVABLE_ASSERT( false );
    return 0;
}

ModelValue::operator std::string() const
{
    switch( mType )
    {
    case Mt_String:
        return ((get_string_t*)mValue)->operator ()();
    case Mt_Int:
        return boost::lexical_cast<std::string>( operator int32_t() );
    case Mt_Double:
        return boost::lexical_cast<std::string>( operator double() );
    default:
        break;
    }
    OBSERVABLE_ASSERT( false );
    return std::string();
}

void ModelValue::operator()() const
{
    if( mType == Mt_VoidFunction && mValue )
    {
        return ( *( void_function_t* )mValue )();
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( int32_t Arg ) const
{
    if( mType == Mt_IntFunction && mValue )
    {
        return ( *( int_function_t* )mValue )( Arg );
    }
    else if( mType == Mt_DoubleFunction && mValue )
    {
        return ( *( double_function_t* )mValue )( ( double )Arg );
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( double Arg ) const
{
    if( mType == Mt_DoubleFunction && mValue )
    {
        return ( *( double_function_t* )mValue )( Arg );
    }
    else if( mType == Mt_IntFunction && mValue )
    {
        return ( *( int_function_t* )mValue )( ( int32_t )Arg );
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( std::string const& Arg ) const
{
    if( mType == Mt_StringFunction && mValue )
    {
        return ( *( string_function_t* )mValue )( Arg );
    }
    OBSERVABLE_ASSERT( false );
}

RootModel::RootModel()
    : ModelValue( "root", NULL )
{

}

#undef OBSERVABLE_ASSERT

} // namespace platform


