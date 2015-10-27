#ifndef INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H

#include "component.h"

class IScoreOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IScoreOnDeathComponent)
    virtual void SetScored(bool scored)=0;
    virtual bool IsScored()const=0;
};

#endif//INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_score_on_death_component" -m "bool-scored"
