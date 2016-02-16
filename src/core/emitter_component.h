#ifndef INCLUDED_CORE_EMITTER_COMPONENT_H
#define INCLUDED_CORE_EMITTER_COMPONENT_H

#include "i_emitter_component.h"
#include "core/property_loader.h"
#include <boost/serialization/vector.hpp>

class EmitterComponent : public IEmitterComponent
{
public:
    EmitterComponent();
    virtual std::vector<int32_t> GetEmitTypes() const;
    virtual void Emitted(std::vector<int32_t> emitTypes);
    virtual void Update( double dt );
private:
    friend class ComponentFactory;
    friend class EmitterComponentLoader;
    struct EmitDesc {
        int32_t mEmitType;
        double mDelay;
        double mDelayVariance;
        double mCooldown;
        double mProbability;
        int32_t mIteration; //number of times when actually emitter emits
        int32_t mIterationVariance;
        int32_t mIterationCurrent; //number of times of emitted stuff
        EmitDesc();
        friend class ::boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version);
    };

    typedef std::vector<EmitDesc> EmitDescs;
    void InitEmitDescs(EmitDescs emitDescs);
    EmitDescs mEmitDescs;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void EmitterComponent::EmitDesc::serialize(Archive& ar, const unsigned int version)
{
    ar & mEmitType;
    ar & mDelay;
    ar & mDelayVariance;
    ar & mCooldown;
    ar & mProbability;
    ar & mIteration;
    ar & mIterationVariance;
    ar & mIterationCurrent;
}

template<class Archive>
void EmitterComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IEmitterComponent>(*this);
    ar & mEmitDescs;
}

class EmitterComponentLoader : public ComponentLoader<EmitterComponent>
{
    virtual void BindValues();
protected:
    EmitterComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_EMITTER_COMPONENT_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "component" -c "emitter_component" -m "int32_t-emitType bool-isemitting double-frequency"
