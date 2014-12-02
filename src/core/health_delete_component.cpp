#include "health_delete_component.h"

HealthDeleteComponent::HealthDeleteComponent()
    : HealthComponent()
{
}

void HealthDeleteComponent::Update( double Seconds )
{
    if( !IsAlive() )
    {
        mNeedDelete = true;
    }
}

void HealthDeleteComponentLoader::BindValues()
{
}

HealthDeleteComponentLoader::HealthDeleteComponentLoader()
{
    SetBase<HealthComponentLoader>();
}
