#ifndef INCLUDED_CORE_LUCKY_ROCKET_H
#define INCLUDED_CORE_LUCKY_ROCKET_H
#include "core/weapon.h"

class LuckyRocket :	public Weapon
{
    LuckyRocket(int32_t Id);
    friend class Factory<Item>;
public:
    virtual glm::vec3 GetMouseColor() const;
    virtual std::string GetMouseText() const;
};

#endif // INCLUDED_CORE_LUCKY_ROCKET_H
