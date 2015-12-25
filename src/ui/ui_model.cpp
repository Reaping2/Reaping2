#include "ui_model.h"

namespace ui {

UiModel::Val::Val()
    : base( 0 )
{
}

platform::ModelValue* UiModel::CreateModel( std::string const& name )
{
    BOOST_ASSERT( name.find( '.' ) == std::string::npos );
    ModelValue* m = new ModelValue( (get_int_t) boost::lambda::bind( &UiModel::Value, this, name ), name, this );
    mOwnedModels.push_back( m );
    mOwnedCallModels.push_back( new ModelValue( (int_function_t) boost::lambda::bind( &UiModel::Add, this, name, boost::lambda::_1 ), "add", m ) );
    return m;
}

platform::ModelValue* UiModel::FindSubModel( std::string const& name ) const
{
    ModelMap_t::const_iterator i = mModels.find( name );
    if( i == mModels.end() )
    {
        return const_cast<UiModel*>( this )->CreateModel( name );
    }
    return i->second;
}

UiModel::UiModel()
    : ModelValue( "ui", &platform::RootModel::Get() )
{
}

UiModel::~UiModel()
{
    mOwnedCallModels.clear();
    mOwnedModels.clear();
}

int32_t UiModel::Value( std::string const& name )
{
    return mValues[ name ].base;
}

void UiModel::Add( std::string const& name, int32_t val )
{
    Val& v = mValues[ name ];
    v.base += val;
}



}


