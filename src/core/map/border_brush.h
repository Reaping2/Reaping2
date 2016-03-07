#ifndef INCLUDED_CORE_MAP_BORDER_BRUSH_H
#define INCLUDED_CORE_MAP_BORDER_BRUSH_H
#include "platform/i_platform.h"
#include "i_brush.h"
#include "../scene.h"
#include "neighbors.h"
#include "../i_border_component.h"

namespace map {

class BorderBrush: public IBrush
{
public:
    BorderBrush( int32_t Id );
    virtual void Update( double DeltaTime );

    void UpdateBorders( Neighbors& neighbors );

protected:
    typedef std::vector<Neighbors::NeighborMap_t> NeighborDirMap_t;
    NeighborDirMap_t mNeighborDirMap;
    NeighborDirMap_t mNeighborOuterDirMap;
    IBorderComponent::Borders_t GetBorders( Neighbors& neighbors, NeighborDirMap_t& neighborDirs );
    bool mMouseLeftPressed;
    bool mMouseRightPressed;
};

} // namespace map
#endif//INCLUDED_CORE_MAP_BORDER_BRUSH_H