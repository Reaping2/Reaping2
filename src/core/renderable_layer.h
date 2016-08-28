#ifndef INCLUDED_CORE_RENDERABLE_LAYER_H
#define INCLUDED_CORE_RENDERABLE_LAYER_H
#include "platform/singleton.h"
#include <map>

class RenderableLayer : public platform::Singleton<RenderableLayer>
{
protected:
    friend class platform::Singleton<RenderableLayer>;
    RenderableLayer();
public:
    enum Type
    {
        Background = 0,
        Background_1,
        Corpses,
        Creeps,
        Players,
        Buildings,
        Num_Layers
    };
    RenderableLayer::Type operator()( int32_t Id ) const;
    typedef std::map<int32_t, RenderableLayer::Type> IdToRendLayerMap_t;
    IdToRendLayerMap_t const& GetIdToRenderLayerMap();
private:
    IdToRendLayerMap_t mIdToRendLayerMap;
};


#endif//INCLUDED_CORE_RENDERABLE_LAYER_H
