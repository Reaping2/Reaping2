#ifndef INCLUDED_CORE_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_RENDERABLE_COMPONENT_H

#include "i_renderable_component.h"
#include "core/property_loader.h"

class Actor;

class RenderableComponentModifier
{
public:
    RenderableComponentModifier(RenderableLayer::Type Lay,int32_t ZOrder);
    void operator()(Opt<Actor>& Obj);

protected:
    RenderableLayer::Type mLayer;
    int32_t mZOrder;
};

class RenderableComponent : public IRenderableComponent
{
public:
    virtual RenderableLayer::Type const& GetLayer()const;
    virtual void SetLayer( RenderableLayer::Type Lay );
    virtual int32_t const& GetZOrder()const;
    virtual void SetZOrder( int32_t ZOrder );
protected:
    RenderableComponent();
    friend class ComponentFactory;
    RenderableLayer::Type mLayer;
    int32_t mZOrder;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

template<class Archive>
void RenderableComponent::serialize(Archive& ar, const unsigned int version)
{
    //NOTE: generated archive for this class
    ar & boost::serialization::base_object<IRenderableComponent>(*this);
    ar & mLayer;
    ar & mZOrder;
}

class RenderableComponentLoader: public ComponentLoader<RenderableComponent>
{
    virtual void BindValues();
protected:
    friend class ComponentLoaderFactory;
    RenderableLayer& mRenderableLayer;
public:
    RenderableComponentLoader();
};
#endif//INCLUDED_CORE_RENDERABLE_COMPONENT_H