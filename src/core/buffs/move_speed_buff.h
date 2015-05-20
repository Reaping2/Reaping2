#ifndef INCLUDED_CORE_MOVE_SPEED_BUFF_H
#define INCLUDED_CORE_MOVE_SPEED_BUFF_H

#include "buff.h"

class MoveSpeedBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(MoveSpeedBuff)
    MoveSpeedBuff();
    virtual void SetFlatBonus(int32_t flatBonus);
    virtual int32_t GetFlatBonus()const;
    virtual void SetPercentBonus(double percentBonus);
    virtual double GetPercentBonus()const;
protected:
    int32_t mFlatBonus;
    double mPercentBonus;
private:
};

#endif//INCLUDED_CORE_MOVE_SPEED_BUFF_H


