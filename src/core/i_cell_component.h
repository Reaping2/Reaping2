#ifndef INCLUDED_CORE_I_CELL_COMPONENT_H
#define INCLUDED_CORE_I_CELL_COMPONENT_H

#include "component.h"
#include "map\level_generator\room_desc.h"

class ICellComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(ICellComponent)
    virtual void SetRoomDesc(Opt<map::RoomDesc> roomDesc)=0;
    virtual Opt<map::RoomDesc> GetRoomDesc()const=0;
    virtual void SetX(int32_t x)=0;
    virtual int32_t GetX()const=0;
    virtual void SetY(int32_t y)=0;
    virtual int32_t GetY()const=0;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ICellComponent::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Component>(*this);
}

#endif//INCLUDED_CORE_I_CELL_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "cell_component" -m "Opt<map::RoomDesc>-roomDesc int32_t-x int32_t-y"
