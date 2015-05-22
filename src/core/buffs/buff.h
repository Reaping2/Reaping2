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
    virtual void SetSecsToEnd(double secsToEnd);
    virtual double GetSecsToEnd()const;
    virtual void SetAutoRemove(bool autoRemove);
    virtual bool IsAutoRemove()const;
    virtual void SetUID(int32_t uID);
    virtual int32_t GetUID()const;
protected:
    double mSecsToEnd;
    bool mAutoRemove;
    int32_t mUID;
    Buff();
private:
    static int32_t mNextUId;
};


class DefaultBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(DefaultBuff)
    DefaultBuff();
};

#endif//INCLUDED_CORE_BUFFS_BUFF_H
