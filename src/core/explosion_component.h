#ifndef INCLUDED_CORE_EXPLOSION_COMPONENT_H
#define INCLUDED_CORE_EXPLOSION_COMPONENT_H

#include "i_explosion_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class ExplosionComponent : public IExplosionComponent
{
public:
    virtual void SetStartRadius( double startRadius );
    virtual double GetStartRadius()const;
    virtual void SetMaxRadius( double maxRadius );
    virtual double GetMaxRadius()const;
    virtual void SetScaleSpeed( double scaleSpeed );
    virtual double GetScaleSpeed()const;
protected:
    ExplosionComponent();
    friend class ComponentFactory;
    double mStartRadius;
    double mMaxRadius;
    double mScaleSpeed;
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ExplosionComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IExplosionComponent>( *this );
    ar& mStartRadius;
    ar& mMaxRadius;
    ar& mScaleSpeed;
}

class ExplosionComponentLoader : public ComponentLoader<ExplosionComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( ExplosionComponentLoader )
private:
    virtual void BindValues();
    ExplosionComponentLoader();
protected:
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( ExplosionComponent, ExplosionComponent, "explosion_component" );
#endif//INCLUDED_CORE_EXPLOSION_COMPONENT_H

