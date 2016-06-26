#ifndef INCLUDED_CORE_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_RENDERABLE_COMPONENT_H

#include "i_renderable_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include "platform/serialize_vec4.h"

class Actor;

class RenderableComponentModifier
{
public:
    RenderableComponentModifier( RenderableLayer::Type Lay, int32_t ZOrder, int32_t CastShadow, int32_t ReceiveBlood, int32_t ReceiveShadow );
    void operator()( Opt<Actor>& Obj );

protected:
    RenderableLayer::Type mLayer;
    int32_t mZOrder;
    int32_t mCastShadow;
    int32_t mReceiveBlood;
    int32_t mReceiveShadow;
};

class RenderableComponent : public IRenderableComponent
{
public:
    virtual RenderableLayer::Type const& GetLayer()const;
    virtual void SetLayer( RenderableLayer::Type Lay );
    virtual int32_t const& GetZOrder()const;
    virtual void SetZOrder( int32_t ZOrder );
    virtual int32_t GetCastShadow() const;
    virtual void SetCastShadow( int32_t set );
    virtual int32_t GetReceiveShadow() const;
    virtual void SetReceiveShadow( int32_t set );
    virtual int32_t GetReceiveBlood() const;
    virtual void SetReceiveBlood( int32_t set );
    virtual void SetColor( glm::vec4 const& color );
    virtual glm::vec4 GetColor() const;
protected:
    RenderableComponent();
    friend class ComponentFactory;
    RenderableLayer::Type mLayer;
    int32_t mZOrder;
    int32_t mCastShadow;
    int32_t mReceiveBlood;
    int32_t mReceiveShadow;
    glm::vec4 mColor;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void RenderableComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IRenderableComponent>( *this );
    ar& mLayer;
    ar& mZOrder;
    ar& mCastShadow;
    ar& mReceiveBlood;
    ar& mReceiveShadow;
    ar& mColor;
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

REAPING2_CLASS_EXPORT_KEY2( RenderableComponent, RenderableComponent, "renderable_component" );
#endif//INCLUDED_CORE_RENDERABLE_COMPONENT_H
