#ifndef INCLUDED_CORE_CELL_COMPONENT_H
#define INCLUDED_CORE_CELL_COMPONENT_H

#include "i_cell_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class CellComponent : public ICellComponent
{
public:
    CellComponent();
    virtual void SetRoomDesc( Opt<map::RoomDesc> roomDesc );
    virtual Opt<map::RoomDesc> GetRoomDesc() const;
    virtual void SetX( int32_t x );
    virtual int32_t GetX() const;
    virtual void SetY( int32_t y );
    virtual int32_t GetY() const;
protected:
    friend class ComponentFactory;
    Opt<map::RoomDesc> mRoomDesc;
    int32_t mX;
    int32_t mY;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void CellComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<ICellComponent>(*this);
}

class CellComponentLoader : public ComponentLoader<CellComponent>
{
    virtual void BindValues();
protected:
    CellComponentLoader();
    friend class ComponentLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( CellComponent, CellComponent, "cell_component" );

#endif//INCLUDED_CORE_CELL_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "cell_component" -m "Opt<map::RoomDesc>-roomDesc int32_t-x int32_t-y"
