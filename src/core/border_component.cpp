#include "core/border_component.h"

BorderComponent::BorderComponent()
    : mBorders()
{
}

void BorderComponent::SetBorders(Borders_t borders)
{
    mBorders=borders;
}

IBorderComponent::Borders_t BorderComponent::GetBorders()const
{
    return mBorders;
}



void BorderComponentLoader::BindValues()
{
    IBorderComponent::Borders_t borders;
    Json::Value const& json = (*mSetters)["borders"];
    if( !json.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
    {
        Json::Value& part = *i;
        BorderType::Type typ = BorderType::Get()( AutoId( part.asString() ) );
        borders.push_back( typ );
    }
    Bind<IBorderComponent::Borders_t>( &BorderComponent::SetBorders, borders );

}

BorderComponentLoader::BorderComponentLoader()
{
}
