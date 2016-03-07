#ifndef INCLUDED_CORE_CLOAK_BUFF_H
#define INCLUDED_CORE_CLOAK_BUFF_H

#include "buff.h"

class CloakBuff : public Buff
{
public:
    DEFINE_BUFF_BASE( CloakBuff )
    CloakBuff();
    virtual void SetApplied( bool applied );
    virtual bool IsApplied()const;
protected:
    bool mApplied;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void CloakBuff::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<Buff>( *this );
    ar& mApplied;
}

#endif//INCLUDED_CORE_CLOAK_BUFF_H

//command:  "classgenerator.exe" -g "buff" -c "cloak_buff" -m "bool-applied"
