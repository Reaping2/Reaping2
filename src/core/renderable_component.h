#ifndef INCLUDED_CORE_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_RENDERABLE_COMPONENT_H

#include "i_renderable_component.h"
#include "core/property_loader.h"
#include "platform/export.h"
#include "platform/serialize_vec4.h"
#include <boost/serialization/vector.hpp>

class Actor;

class RenderableComponentModifier
{
public:
    RenderableComponentModifier( int32_t Lay, int32_t ZOrder, int32_t CastShadow, int32_t ReceiveBlood, int32_t ReceiveShadow );
    void operator()( Opt<Actor>& Obj );

protected:
    int32_t mLayerPriority;
    int32_t mZOrder;
    int32_t mCastShadow;
    int32_t mReceiveBlood;
    int32_t mReceiveShadow;
};

class RenderableComponent : public IRenderableComponent
{
public:
    virtual int32_t const& GetLayerPriority()const;
    virtual void SetLayerPriority( int32_t Lay );
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
    virtual void SetRandomSprites( RandomSprites_t const& randomSprites );
    virtual RandomSprites_t const& GetRandomSprites() const;
    virtual void SetSpriteIndex( int32_t spriteIndex );
    virtual int32_t GetSpriteIndex() const;
    virtual void SetShaderId( int32_t id );
    virtual int32_t GetShaderId()const;
    virtual void SetPostProcessIds( std::vector<int32_t> const& ids );
    virtual std::vector<int32_t>const& GetPostProcessIds() const;
protected:
    RenderableComponent();
    friend class ComponentFactory;
    int32_t mLayerPriority;
    int32_t mZOrder;
    int32_t mCastShadow;
    int32_t mReceiveBlood;
    int32_t mReceiveShadow;
    glm::vec4 mColor;
    RandomSprites_t mRandomSprites;
    int32_t mSpriteIndex;
    int32_t mShaderId;
    std::vector<int32_t> mPostprocessorIds;
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
    ar& mLayerPriority;
    ar& mZOrder;
    ar& mCastShadow;
    ar& mReceiveBlood;
    ar& mReceiveShadow;
    ar& mColor;
    ar& mRandomSprites;
    ar& mSpriteIndex;
    ar& mShaderId;
    ar& mPostprocessorIds;
}

class RenderableComponentLoader: public ComponentLoader<RenderableComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( RenderableComponentLoader )
private:
    virtual void BindValues();
protected:
    friend class ComponentLoaderFactory;
    RenderableLayer& mRenderableLayer;
public:
    RenderableComponentLoader();
};

REAPING2_CLASS_EXPORT_KEY2( RenderableComponent, RenderableComponent, "renderable_component" );
#endif//INCLUDED_CORE_RENDERABLE_COMPONENT_H
