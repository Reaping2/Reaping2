#include "i_platform.h"

#ifdef ENABLE_MODEL_ASSERTS
#define OBSERVABLE_ASSERT(x) assert(x)
#else
#define OBSERVABLE_ASSERT(x) (void(0))
#endif

ModelValue::ModelValue( int32_t const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_Int )
    , mValue( ModelFor )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( std::string const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_String )
    , mValue( ModelFor )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( void_function_t const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_VoidFunction )
    , mValue( new void_function_t( ModelFor ) )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( int_function_t const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_IntFunction )
    , mValue( new int_function_t( ModelFor ) )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( double_function_t const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_DoubleFunction )
    , mValue( new double_function_t( ModelFor ) )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( string_function_t const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_StringFunction )
    , mValue( new string_function_t( ModelFor ) )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( std::string const& Name, ModelValue* Parent )
    : mType( Mt_Multi )
    , mParent( Parent )
{
    if( !mParent )
    {
        return;
    }
    std::pair<ModelMap_t::iterator, bool> Inserted = mParent->mModels.insert( std::make_pair( Name, this ) );
    OBSERVABLE_ASSERT( Inserted.second );
}

ModelValue::ModelValue( double const& ModelFor, std::string const& Name, ModelValue* Parent )
    : mType( Mt_Double )
    , mValue( ModelFor )
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
    case Mt_DoubleFunction:
        delete ( double_function_t* )mValue.f;
        break;
    case Mt_IntFunction:
        delete ( int_function_t* )mValue.f;
        break;
    case Mt_StringFunction:
        delete ( string_function_t* )mValue.f;
        break;
    case Mt_VoidFunction:
        delete ( void_function_t* )mValue.f;
        break;
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
                return DefaultModelValue::Get();
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
        return *( mValue.i );
    }
    OBSERVABLE_ASSERT( false );
    return 0;
}

ModelValue::operator double() const
{
    if( mType == Mt_Double )
    {
        return *( mValue.d );
    }
    OBSERVABLE_ASSERT( false );
    return 0;
}

ModelValue::operator std::string() const
{
    switch( mType )
    {
    case Mt_String:
        return *( mValue.s );
    case Mt_Int:
        return boost::lexical_cast<std::string>( *mValue.i );
    case Mt_Double:
        return boost::lexical_cast<std::string>( *mValue.d );
    default:
        break;
    }
    OBSERVABLE_ASSERT( false );
    return std::string();
}

void ModelValue::operator()() const
{
    if( mType == Mt_VoidFunction && mValue.f )
    {
        return ( *( void_function_t* )mValue.f )();
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( int32_t Arg ) const
{
    if( mType == Mt_IntFunction && mValue.f )
    {
        return ( *( int_function_t* )mValue.f )( Arg );
    }
    else if( mType == Mt_DoubleFunction && mValue.f )
    {
        return ( *( double_function_t* )mValue.f )( ( double )Arg );
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( double Arg ) const
{
    if( mType == Mt_DoubleFunction && mValue.f )
    {
        return ( *( double_function_t* )mValue.f )( Arg );
    }
    else if( mType == Mt_IntFunction && mValue.f )
    {
        return ( *( int_function_t* )mValue.f )( ( int32_t )Arg );
    }
    OBSERVABLE_ASSERT( false );
}

void ModelValue::operator()( std::string const& Arg ) const
{
    if( mType == Mt_StringFunction && mValue.f )
    {
        return ( *( string_function_t* )mValue.f )( Arg );
    }
    OBSERVABLE_ASSERT( false );
}

RootModel::RootModel()
    : ModelValue( "root", NULL )
{

}

DefaultModelValue::DefaultModelValue()
    : ModelValue( "default", NULL )
{

}
