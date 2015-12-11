#ifndef INCLUDED_CORE_I_KILL_SCORE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_KILL_SCORE_ON_DEATH_COMPONENT_H

#include "component.h"

class IKillScoreOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IKillScoreOnDeathComponent)
    virtual void SetKillerGUID(int32_t killerGUID)=0;
    virtual int32_t GetKillerGUID()const=0;
    virtual void SetDeadGUID(int32_t deadGUID)=0;
    virtual int32_t GetDeadGUID()const=0;
    virtual void SetScored(bool scored)=0;
    virtual bool IsScored()const=0;
};

#endif//INCLUDED_CORE_I_KILL_SCORE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_kill_score_on_death_component" -m "int32_t-killerGUID int32_t-deadGUID bool-scored"
