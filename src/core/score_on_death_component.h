#ifndef INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H

#include "i_score_on_death_component.h"
#include "core/property_loader.h"

class ScoreOnDeathComponent : public IScoreOnDeathComponent
{
public:
    ScoreOnDeathComponent();
    virtual void SetScored(bool scored);
    virtual bool IsScored()const;
protected:
    friend class ComponentFactory;
    bool mScored;
private:
};

class ScoreOnDeathComponentLoader : public ComponentLoader<ScoreOnDeathComponent>
{
    virtual void BindValues();
protected:
    ScoreOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "score_on_death_component" -m "bool-scored"
