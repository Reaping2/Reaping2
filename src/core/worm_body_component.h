#ifndef INCLUDED_CORE_WORM_BODY_COMPONENT_H
#define INCLUDED_CORE_WORM_BODY_COMPONENT_H

#include "i_worm_body_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include <boost/serialization/list.hpp>

class WormBodyComponent : public IWormBodyComponent
{
public:
    WormBodyComponent();
    virtual void SetHeadGUID( int32_t headGUID );
    virtual int32_t GetHeadGUID() const;
    virtual void SetBody( bool body );
    virtual bool IsBody() const;
    virtual void SetFollowedGUID( int32_t followedGUID );
    virtual int32_t GetFollowedGUID() const;

protected:
    friend class ComponentFactory;
    int32_t mHeadGUID;
    bool mBody;
    int32_t mFollowedGUID;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void WormBodyComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<IWormBodyComponent>(*this);
    ar& mHeadGUID;
    ar& mBody;
    ar& mFollowedGUID;
}

class WormBodyComponentLoader : public ComponentLoader<WormBodyComponent>
{
    virtual void BindValues();
protected:
    WormBodyComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( WormBodyComponent, WormBodyComponent, "worm_body_component" );

#endif//INCLUDED_CORE_WORM_BODY_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "worm_body_component" -m "int32_t-headGUID bool-body int32_t-followedGUID PrevPositions_t-prevPositions"
