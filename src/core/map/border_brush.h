#ifndef INCLUDED_CORE_MAP_BORDER_BRUSH_H
#define INCLUDED_CORE_MAP_BORDER_BRUSH_H
#include "i_brush.h"
#include "neighbors.h"

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
