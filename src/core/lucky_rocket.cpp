#include "core/lucky_rocket.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

LuckyRocket::LuckyRocket( int32_t Id )
    : Weapon( Id )
{
}

LuckyRocket::LuckyRocket()
    : Weapon(-1)
{
}

glm::vec3 LuckyRocket::GetMouseColor() const
{
    double t = glfwGetTime();
    return mReloadTime>0.0&&mStaticReload==0.0?
        glm::vec3( 1.0, 0.0, 0.0 ):glm::vec3( sin( t ) * sin( t ) / 2., cos( t ) * cos( t ), 1 - std::abs( sin( t ) ) );
}

std::string LuckyRocket::GetMouseText() const
{
    return mReloadTime>0.0 ? "Not so lucky!" : Weapon::GetMouseText();
}


void LuckyRocketLoader::BindValues()
{
}

LuckyRocketLoader::LuckyRocketLoader()
{
    SetBase<WeaponLoader>();
}

REAPING2_CLASS_EXPORT_IMPLEMENT(LuckyRocket, LuckyRocket);
