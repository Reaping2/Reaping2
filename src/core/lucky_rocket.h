#ifndef INCLUDED_CORE_LUCKY_ROCKET_H
#define INCLUDED_CORE_LUCKY_ROCKET_H
#include "core/weapon.h"
#include "platform/export.h"

class LuckyRocket :	public Weapon
{
    LuckyRocket(int32_t Id);
    LuckyRocket();

    void InitMembers();

    friend class Factory<Item>;
public:
    virtual glm::vec3 GetMouseColor() const;
    virtual std::string GetMouseText() const;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void LuckyRocket::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Weapon>(*this);
}


REAPING2_CLASS_EXPORT_KEY2(LuckyRocket, LuckyRocket,"lucky_rocket");
#endif // INCLUDED_CORE_LUCKY_ROCKET_H
