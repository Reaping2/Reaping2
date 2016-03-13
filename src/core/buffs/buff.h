#ifndef INCLUDED_CORE_BUFFS_BUFF_H
#define INCLUDED_CORE_BUFFS_BUFF_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "platform/export.h"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>

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
    class SecsToEndModifier
    {
    public:
        SecsToEndModifier( double secsToEnd );
        void operator()(Opt<Buff>& Obj);
    protected:
        double mSecsToEnd;
    };

    virtual int GetType() const = 0;
    virtual ~Buff();
    virtual void SetSecsToEnd( double secsToEnd );
    virtual double GetSecsToEnd()const;
    virtual void SetAutoRemove( bool autoRemove );
    virtual bool IsAutoRemove()const;
    virtual void SetUID( int32_t uID );
    virtual int32_t GetUID()const;
protected:
    double mSecsToEnd;
    bool mAutoRemove;
    int32_t mUID;
    Buff();
private:
    static int32_t mNextUId;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void Buff::serialize( Archive& ar, const unsigned int version )
{
    ar& mSecsToEnd;
    ar& mAutoRemove;
    ar& mUID;
}

class DefaultBuff : public Buff
{
public:
    DEFINE_BUFF_BASE( DefaultBuff )
    DefaultBuff();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void DefaultBuff::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Buff>( *this );
}

#endif//INCLUDED_CORE_BUFFS_BUFF_H
