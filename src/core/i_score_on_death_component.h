#ifndef INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H

#include "component.h"
#include "platform/export.h"

class IScoreOnDeathComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IScoreOnDeathComponent )
    virtual void SetScored( bool scored ) = 0;
    virtual bool IsScored()const = 0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IScoreOnDeathComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IScoreOnDeathComponent, IScoreOnDeathComponent, "i_score_on_death_component" );
#endif//INCLUDED_CORE_I_SCORE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "i_component" -c "i_score_on_death_component" -m "bool-scored"
