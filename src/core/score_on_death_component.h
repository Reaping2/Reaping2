#ifndef INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H

#include "i_score_on_death_component.h"
#include "core/property_loader.h"
#include <boost/serialization/export.hpp>

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
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void ScoreOnDeathComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IScoreOnDeathComponent>(*this);
    ar & mScored;
}

class ScoreOnDeathComponentLoader : public ComponentLoader<ScoreOnDeathComponent>
{
    virtual void BindValues();
protected:
    ScoreOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};


BOOST_CLASS_EXPORT_KEY2(ScoreOnDeathComponent,"score_on_death_component");
#endif//INCLUDED_CORE_SCORE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "score_on_death_component" -m "bool-scored"
