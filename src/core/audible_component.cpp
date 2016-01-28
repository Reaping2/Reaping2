#include "core/audible_component.h"
#include "audible_event.h"
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

void AudibleComponent::AddOneShotEffect( int32_t id )
{
    mEffects.emplace_back( id );
    platform::EventServer<core::AudibleEvent>::Get().SendEvent(
            core::AudibleEvent( mActor->GetGUID(), id, true, true )
            );
}

void AudibleComponent::AddLoopingEffect( int32_t id )
{
    auto it = std::find_if( std::begin( mEffects ), std::end( mEffects ), [&]( AudibleEffectDesc const& d ) { return d.Id == id && d.TTL != AudibleEffectDesc::TTL_Infinity; } );
    if( std::end( mEffects ) == it )
    {
        AudibleEffectDesc d( id );
        d.TTL = 2;
        mEffects.push_back( d );
        platform::EventServer<core::AudibleEvent>::Get().SendEvent(
                core::AudibleEvent( mActor->GetGUID(), id, false, true )
                );
    }
    else
    {
        it->TTL = 2;
    }
}

namespace {
void InitAudibleEffectDescs( std::vector<int32_t> const& ids, std::vector<AudibleEffectDesc>& descs )
{
    std::for_each( std::begin( ids ), std::end( ids ),
        [&]( int32_t id ) { descs.emplace_back( id ); } );
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
