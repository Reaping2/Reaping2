#include "core/audible_component.h"
#include <boost/lambda/lambda.hpp>

AudibleComponent::AudibleComponent()
{
}

std::vector<AudibleEffectDesc> const& AudibleComponent::GetEffects()const
{
    return mEffects;
}

std::vector<AudibleEffectDesc>& AudibleComponent::GetEffects()
{
    return mEffects;
}

namespace {
void InitAudibleEffectDescs( std::vector<int32_t> const& ids, std::vector<AudibleEffectDesc>& descs )
{
    static int32_t UID = 0;
    std::for_each( std::begin( ids ), std::end( ids ),
        [&]( int32_t id ) { descs.emplace_back( ++UID, id ); } );
}
}

void AudibleComponentLoader::BindValues()
{
    Json::Value const& json = (*mSetters)["sounds"];
    if( !json.isArray() )
    {
        return;
    }
    std::vector<int32_t> ids;
    for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
    {
        Json::Value& part = *i;
        std::string str;
        Json::GetStr( part["name"], str );
        ids.push_back( AutoId( str ) );
    }
    Add( boost::lambda::bind( &InitAudibleEffectDescs, ids, boost::lambda::_1 ->* &AudibleComponent::mEffects ) );
}

AudibleComponentLoader::AudibleComponentLoader()
{
}
