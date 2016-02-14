#ifndef INCLUDED_CORE_ARMOR_BUFF_H
#define INCLUDED_CORE_ARMOR_BUFF_H

#include "buff.h"

class ArmorBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(ArmorBuff)
    ArmorBuff();
    virtual void SetApplied(bool applied);
    virtual bool IsApplied()const;
    virtual void SetArmorMax(int32_t armorMax);
    virtual int32_t GetArmorMax()const;
protected:
    bool mApplied;
    int32_t mArmorMax;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ArmorBuff::serialize(Archive& ar, const unsigned int version)
{
    ar & boost::serialization::base_object<Buff>(*this);
    ar & mApplied;
    ar & mArmorMax;
}

#endif//INCLUDED_CORE_ARMOR_BUFF_H

//command:  "classgenerator.exe" -g "buff" -c "armor_buff" -m "bool-applied int32_t-armorMax"
