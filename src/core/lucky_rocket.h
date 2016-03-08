#ifndef INCLUDED_CORE_LUCKY_ROCKET_H
#define INCLUDED_CORE_LUCKY_ROCKET_H
#include "core/weapon.h"
#include "platform/export.h"

class LuckyRocket : public Weapon
{
public:
    LuckyRocket( int32_t Id );
    LuckyRocket();
    friend class Factory<Item>;
    virtual glm::vec3 GetMouseColor() const;
    virtual std::string GetMouseText() const;
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void LuckyRocket::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Weapon>( *this );
}

class LuckyRocketLoader: public ItemLoader<LuckyRocket>
{
public:
    virtual void BindValues();
    LuckyRocketLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( LuckyRocket, LuckyRocket, "lucky_rocket" );
#endif // INCLUDED_CORE_LUCKY_ROCKET_H
