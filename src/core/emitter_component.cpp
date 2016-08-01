#include "core/emitter_component.h"
#include <boost/lambda/lambda.hpp>
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

EmitterComponent::EmitterComponent()
{
}

std::vector<int32_t> EmitterComponent::GetEmitTypes() const
{
    std::vector<int32_t> rv;
    for( EmitDescs::const_iterator i = mEmitDescs.begin(), e = mEmitDescs.end(); i != e; ++i )
    {
        EmitDesc const& d = *i;
        if ( d.mIteration != -1 && d.mIterationCurrent >= d.mIteration )
        {
            continue;
        }
        if( d.mCooldown > 0.0 )
        {
            continue;
        }
        if( ( RandomGenerator::global()() % 100 / 100. ) >= d.mProbability )
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
            d.mCooldown = d.mDelay * ( 1. - d.mDelayVariance / 2. )  + d.mDelayVariance * ( RandomGenerator::global()() % 100 ) / 100.;
            continue;
        }
        d.mDelay -= dt;
    }
}

void EmitterComponent::InitEmitDescs( EmitDescs emitDescs )
{
    mEmitDescs = emitDescs;
    for( EmitDescs::iterator i = mEmitDescs.begin(), e = mEmitDescs.end(); i != e; ++i )
    {
        EmitDesc& d = *i;
        if ( d.mIteration != -1 )
        {
            d.mIteration = std::max<int>( 0, d.mIteration + 2 * d.mIterationVariance * ( RandomGenerator::global()() % 100 ) / 100 - d.mIterationVariance );
        }
    }
}

void EmitterComponent::Emitted( std::vector<int32_t> emitTypes )
{
    for( EmitDescs::iterator i = mEmitDescs.begin(), e = mEmitDescs.end(); i != e; ++i )
    {
        EmitDesc& d = *i;
        if ( std::find( emitTypes.begin(), emitTypes.end(), d.mEmitType ) != emitTypes.end() )
        {
            ++d.mIterationCurrent;
        }
    }
}

EmitterComponent::EmitDesc::EmitDesc()
    : mEmitType( 0 )
    , mDelay( 0 )
    , mDelayVariance( 0 )
    , mCooldown( 0 )
    , mProbability( 1 )
    , mIteration( -1 )
    , mIterationVariance( 0 )
    , mIterationCurrent( 0 )
{
}

void EmitterComponentLoader::BindValues()
{
    EmitterComponent::EmitDescs descs;
    Json::Value const& json = ( *mSetters )["emit_descs"];
    if( !json.isArray() )
    {
        return;
    }
    for( Json::Value::iterator i = json.begin(), e = json.end(); i != e; ++i )
    {
        Json::Value& part = *i;
        EmitterComponent::EmitDesc d;
        Json::GetDouble( part["delay"], d.mDelay );
        Json::GetDouble( part["delay_variance"], d.mDelayVariance );
        Json::GetDouble( part["probability"], d.mProbability );
        std::string str;
        Json::GetStr( part["emit_type"], str );
        d.mEmitType = AutoId( str );
        Json::GetInt( part["iteration"], d.mIteration );
        Json::GetInt( part["iteration_variance"], d.mIterationVariance );
        descs.push_back( d );
    }
    Bind<EmitterComponent::EmitDescs>( &EmitterComponent::InitEmitDescs, descs );
    //Add( boost::lambda::_1 ->* &EmitterComponent::mEmitDescs = descs );
}

EmitterComponentLoader::EmitterComponentLoader()
{
}

REAPING2_CLASS_EXPORT_IMPLEMENT( EmitterComponent, EmitterComponent );
