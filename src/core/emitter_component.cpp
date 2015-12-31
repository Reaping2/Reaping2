#include "core/emitter_component.h"

EmitterComponent::EmitterComponent()
{
}

std::vector<int32_t> EmitterComponent::GetEmitTypes() const
{
    std::vector<int32_t> rv;
    for( EmitDescs::const_iterator i = mEmitDescs.begin(), e = mEmitDescs.end(); i != e; ++i )
    {
        EmitDesc const& d = *i;
        if( d.mCooldown > 0.0 )
        {
            continue;
        }
        if( ( rand() % 100 / 100. ) > d.mProbability )
        {
            continue;
        }
        rv.push_back( d.mEmitType );
    }
    return rv;
}

void EmitterComponent::Update( double dt )
{
    for( EmitDescs::iterator i = mEmitDescs.begin(), e = mEmitDescs.end(); i != e; ++i )
    {
        EmitDesc& d = *i;
        if( d.mCooldown < 0.0 )
        {
            d.mCooldown = d.mDelay * ( 1. - d.mDelayVariance / 2. )  + d.mDelayVariance * ( rand() % 100 ) / 100.;
            continue;
        }
        d.mDelay -= dt;
    }
}

EmitterComponent::EmitDesc::EmitDesc()
    : mEmitType( 0 )
    , mDelay( 0 )
    , mDelayVariance( 0 )
    , mCooldown( 0 )
    , mProbability( 1 )
{
}

void EmitterComponent::Init( Json::Value const& json )
{
    if( !json.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
    {
        Json::Value& part = *i;
        EmitDesc d;
        Json::GetDouble( part["delay"], d.mDelay );
        Json::GetDouble( part["delay_variance"], d.mDelayVariance );
        Json::GetDouble( part["probability"], d.mProbability );
        std::string str;
        Json::GetStr( part["emit_type"], str );
        d.mEmitType = AutoId( str );
        mEmitDescs.push_back( d );
    }
}

void EmitterComponentLoader::BindValues()
{
    Bind<Json::Value>( &EmitterComponent::Init, (*mSetters)["emit_descs"] );
}

EmitterComponentLoader::EmitterComponentLoader()
{
}
