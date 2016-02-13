#ifndef INCLUDED_CORE_DETONATE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_DETONATE_ON_HIT_COMPONENT_H

#include "i_detonate_on_hit_component.h"
#include "core/property_loader.h"

class DetonateOnHitComponent : public IDetonateOnHitComponent
{
public:
    DetonateOnHitComponent();
    virtual void SetMaterial(int32_t material);
    virtual int32_t GetMaterial()const;
    virtual void SetAddRadius(double addRadius);
    virtual double GetAddRadius()const;
    virtual void SetRemoveOnHit(bool removeOnHit);
    virtual bool IsRemoveOnHit()const;
protected:
    friend class ComponentFactory;
    int32_t mMaterial;
    double mAddRadius;
    bool mRemoveOnHit;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void DetonateOnHitComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IDetonateOnHitComponent>(*this);
    ar & mMaterial;
    ar & mAddRadius;
    ar & mRemoveOnHit;
}

class DetonateOnHitComponentLoader : public ComponentLoader<DetonateOnHitComponent>
{
    virtual void BindValues();
protected:
    DetonateOnHitComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_DETONATE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "detonate_on_hit_component" -m "int32_t-material double-addRadius bool-removeOnHit"
