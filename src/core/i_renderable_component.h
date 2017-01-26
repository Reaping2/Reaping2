#ifndef INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#define INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
#include "core/component.h"
#include "core/renderable_layer.h"
#include "platform/export.h"

class IRenderableComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( IRenderableComponent )
    virtual int32_t const& GetLayerPriority()const = 0;
    virtual void SetLayerPriority( int32_t Lay ) = 0;
    virtual int32_t const& GetZOrder()const = 0;
    virtual void SetZOrder( int32_t ZOrder ) = 0;
    virtual int32_t GetCastShadow() const = 0;
    virtual void SetCastShadow( int32_t set ) = 0;
    virtual int32_t GetReceiveShadow() const = 0;
    virtual void SetReceiveShadow( int32_t set ) = 0;
    virtual int32_t GetReceiveBlood() const = 0;
    virtual void SetReceiveBlood( int32_t set ) = 0;
    virtual void SetColor( glm::vec4 const& color ) = 0;
    virtual glm::vec4 GetColor()const = 0;
    typedef std::vector<int32_t> RandomSprites_t; //weight of sprites
    virtual void SetRandomSprites( RandomSprites_t const& randomSprites ) = 0;
    virtual RandomSprites_t const& GetRandomSprites()const = 0;
    virtual void SetSpriteIndex( int32_t spriteIndex ) = 0;
    virtual int32_t GetSpriteIndex()const = 0;
    virtual void SetShaderId( int32_t id ) = 0;
    virtual int32_t GetShaderId()const = 0;
    virtual void SetPostProcessIds( std::vector<int32_t> const& ids ) =0;
    virtual std::vector<int32_t>const& GetPostProcessIds() const=0;
protected:
    friend class ComponentFactory;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void IRenderableComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( IRenderableComponent, IRenderableComponent, "i_renderable_component" );
#endif//INCLUDED_CORE_I_RENDERABLE_COMPONENT_H
