#ifndef INCLUDED_CORE_CLOAK_BUFF_H
#define INCLUDED_CORE_CLOAK_BUFF_H

#include "buff.h"

class CloakBuff : public Buff
{
public:
    DEFINE_BUFF_BASE(CloakBuff)
    CloakBuff();
    virtual void SetApplied(bool applied);
    virtual bool IsApplied()const;
protected:
    bool mApplied;
private:
};

#endif//INCLUDED_CORE_CLOAK_BUFF_H

//command:  "classgenerator.exe" -g "buff" -c "cloak_buff" -m "bool-applied"
