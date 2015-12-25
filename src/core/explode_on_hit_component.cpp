#include "core/explode_on_hit_component.h"

ExplodeOnHitComponent::ExplodeOnHitComponent()
    : mExplosionProjectile(-1)
    , mCount(1)
    , mScatter(0.0)
    , mDetonatorMaterial(-1)
    , mAddRadius(0.0)
    , mExploded(false)
{
}

void ExplodeOnHitComponent::SetExplosionProjectile(int32_t explosionProjectile)
{
    mExplosionProjectile=explosionProjectile;
}

int32_t ExplodeOnHitComponent::GetExplosionProjectile()const
{
    return mExplosionProjectile;
}

void ExplodeOnHitComponent::SetCount(int32_t count)
{
    mCount=count;
}

int32_t ExplodeOnHitComponent::GetCount()const
{
    return mCount;
}

void ExplodeOnHitComponent::SetScatter(double scatter)
{
    mScatter=scatter;
}

double ExplodeOnHitComponent::GetScatter()const
{
    return mScatter;
}

void ExplodeOnHitComponent::SetDetonatorMaterial(int32_t detonatorMaterial)
{
    mDetonatorMaterial=detonatorMaterial;
}

int32_t ExplodeOnHitComponent::GetDetonatorMaterial()const
{
    return mDetonatorMaterial;
}

void ExplodeOnHitComponent::SetAddRadius(double addRadius)
{
    mAddRadius=addRadius;
}

double ExplodeOnHitComponent::GetAddRadius()const
{
    return mAddRadius;
}

void ExplodeOnHitComponent::SetExploded(bool exploded)
{
    mExploded=exploded;
}

bool ExplodeOnHitComponent::IsExploded()const
{
    return mExploded;
}


void ExplodeOnHitComponentLoader::BindValues()
{
    std::string istr;
    if( Json::GetStr( (*mSetters)["explosion_projectile"], istr))
    {
        Bind<int32_t>(&ExplodeOnHitComponent::SetExplosionProjectile,AutoId(istr));
    }
    Bind("count",func_int32_t(&ExplodeOnHitComponent::SetCount));
    Bind("scatter",func_double(&ExplodeOnHitComponent::SetScatter));
    
    if( Json::GetStr( (*mSetters)["detonator_material"], istr))
    {
        Bind<int32_t>(&ExplodeOnHitComponent::SetDetonatorMaterial,AutoId(istr));
    }
    Bind("add_radius",func_int32_t(&ExplodeOnHitComponent::SetAddRadius));
}

ExplodeOnHitComponentLoader::ExplodeOnHitComponentLoader()
{
}
