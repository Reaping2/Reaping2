#ifndef INCLUDED_CORE_MAX_HEALTH_BUFF_H
#define INCLUDED_CORE_MAX_HEALTH_BUFF_H

#include "buff.h"

class MaxHealthBuff : public Buff
{
public:
    DEFINE_BUFF_BASE( MaxHealthBuff )
    MaxHealthBuff();
    virtual void SetFlatBonus( int32_t flatBonus );
    virtual int32_t GetFlatBonus()const;
    virtual void SetPercentBonus( double percentBonus );
    virtual double GetPercentBonus()const;
protected:
    int32_t mFlatBonus;
    double mPercentBonus;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void MaxHealthBuff::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Buff>( *this );
    ar& mFlatBonus;
    ar& mPercentBonus;
}

#endif//INCLUDED_CORE_MAX_HEALTH_BUFF_H

