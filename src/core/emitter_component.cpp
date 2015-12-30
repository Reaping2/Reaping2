#include "core/emitter_component.h"

EmitterComponent::EmitterComponent()
    : mEmitType(0)
    , mFrequency(0.0)
    , mCooldown(0.0)
{
}

void EmitterComponent::SetEmitType(int32_t emitType)
{
    mEmitType=emitType;
}

int32_t EmitterComponent::GetEmitType()const
{
    return mEmitType;
}

bool EmitterComponent::IsEmitting()const
{
    return mCooldown <= 0.0;
}

void EmitterComponent::SetFrequency(double frequency)
{
    mFrequency=frequency;
}

double EmitterComponent::GetFrequency()const
{
    return mFrequency;
}

void EmitterComponent::Update( double dt )
{
    if( mCooldown < 0.0 )
    {
        mCooldown = mFrequency * ( 0.75 + (rand() % 50) / 10. );
        return;
    }
    mCooldown -= dt;
}

void EmitterComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["emit_type"], istr))
    {
        Bind<int32_t>( &EmitterComponent::SetEmitType,AutoId(istr) );
    }
    Bind( "frequency",func_double(&EmitterComponent::SetFrequency) );
}

EmitterComponentLoader::EmitterComponentLoader()
{
}
