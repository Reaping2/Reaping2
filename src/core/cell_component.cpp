#include "core/cell_component.h"

CellComponent::CellComponent()
    : mRoomDesc()
    , mX(0)
    , mY(0)
{
}

void CellComponent::SetRoomDesc( Opt<map::RoomDesc> roomDesc )
{
    mRoomDesc = roomDesc;
}

Opt<map::RoomDesc> CellComponent::GetRoomDesc() const
{
    return mRoomDesc;
}

void CellComponent::SetX( int32_t x )
{
    mX = x;
}

int32_t CellComponent::GetX() const
{
    return mX;
}

void CellComponent::SetY( int32_t y )
{
    mY = y;
}

int32_t CellComponent::GetY() const
{
    return mY;
}



void CellComponentLoader::BindValues()
{
}

CellComponentLoader::CellComponentLoader()
{
}


REAPING2_CLASS_EXPORT_IMPLEMENT( CellComponent, CellComponent );
