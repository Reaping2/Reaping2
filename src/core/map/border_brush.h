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

    bool mMouseLeftPressed;
    bool mMouseRightPressed;
};

} // namespace map
#endif//INCLUDED_CORE_MAP_BORDER_BRUSH_H
