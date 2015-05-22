#ifndef INCLUDED_CORE_MAX_HEALTH_BUFF_H
#define INCLUDED_CORE_MAX_HEALTH_BUFF_H

#include "buff.h"

class MaxHealthBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(MaxHealthBuff)
    MaxHealthBuff();
    virtual void SetFlatBonus(int32_t flatBonus);
    virtual int32_t GetFlatBonus()const;
    virtual void SetPercentBonus(double percentBonus);
    virtual double GetPercentBonus()const;
protected:
    int32_t mFlatBonus;
    double mPercentBonus;
private:
};

#endif//INCLUDED_CORE_MAX_HEALTH_BUFF_H

