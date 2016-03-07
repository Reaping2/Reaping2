#ifndef INCLUDED_CORE_MOVE_SPEED_BUFF_H
#define INCLUDED_CORE_MOVE_SPEED_BUFF_H

#include "buff.h"

class MoveSpeedBuff : public Buff
{
public:
    DEFINE_BUFF_BASE( MoveSpeedBuff )
    MoveSpeedBuff();
    virtual void SetFlatBonus( int32_t flatBonus );
    virtual int32_t GetFlatBonus()const;
    virtual void SetPercentBonus( double percentBonus );
    virtual double GetPercentBonus()const;
    virtual void SetRooted( bool rooted );
    virtual bool IsRooted() const;
protected:
    int32_t mFlatBonus;
    double mPercentBonus;
    bool mRooted;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void MoveSpeedBuff::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Buff>( *this );
    ar& mFlatBonus;
    ar& mPercentBonus;
    ar& mRooted;
}

#endif//INCLUDED_CORE_MOVE_SPEED_BUFF_H


