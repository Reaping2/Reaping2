#ifndef INCLUDED_CORE_BUFFS_BUFF_H
#define INCLUDED_CORE_BUFFS_BUFF_H

#include "platform/auto_id.h"
#include "core/opt.h"

#define DEFINE_BUFF_BASE( BuffType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #BuffType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return BuffType::GetType_static(); \
    } \

class Buff 
{
public:
    virtual int GetType() const=0;
    virtual ~Buff();
    virtual double GetSecsToEnd();
    virtual void SetSecsToEnd( double secsToEnd );

protected:
    Buff();
    double mSecsToEnd;
};


class DefaultBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(DefaultBuff)
    DefaultBuff();
};

#endif//INCLUDED_CORE_BUFFS_BUFF_H
