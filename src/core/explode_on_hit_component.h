#ifndef INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

#include "i_explode_on_hit_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ExplodeOnHitComponent : public IExplodeOnHitComponent
{
public:
    ExplodeOnHitComponent();
    virtual void SetDetonatorMaterial( int32_t detonatorMaterial );
    virtual int32_t GetDetonatorMaterial()const;
    virtual void SetAddActorRadius( double addRadius );
    virtual double GetAddRadius()const;
    virtual void SetExploded( bool exploded );
    virtual bool IsExploded()const;
protected:
    friend class ComponentFactory;
    int32_t mDetonatorMaterial;
    double mAddActorRadius;
    bool mExploded;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ExplodeOnHitComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IExplodeOnHitComponent>( *this );
    ar& mDetonatorMaterial;
    ar& mAddActorRadius;
    ar& mExploded;
}

class ExplodeOnHitComponentLoader : public ComponentLoader<ExplodeOnHitComponent>
{
    virtual void BindValues();
protected:
    ExplodeOnHitComponentLoader();
    friend class ComponentLoaderFactory;
    ExplodeDistributionType& mExplodeDistributionType;
};


REAPING2_CLASS_EXPORT_KEY2( ExplodeOnHitComponent, ExplodeOnHitComponent, "explode_on_hit_component" );
#endif//INCLUDED_CORE_EXPLODE_ON_HIT_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_hit_component" -m "int32_t-detonatorMaterial double-addRadius bool-exploded"
