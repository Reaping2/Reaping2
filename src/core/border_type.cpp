#include "core/border_type.h"
#include "platform/auto_id.h"

using platform::AutoId;

BorderType::BorderType()
{
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("top"),BorderType::Top));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("topright"),BorderType::TopRight));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("right"),BorderType::Right));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("bottomright"),BorderType::BottomRight));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("bottom"),BorderType::Bottom));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("bottomleft"),BorderType::BottomLeft));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("left"),BorderType::Left));
    mIdToBorderTypeMap.insert(IdToBorderTypeMap_t::value_type(AutoId("topleft"),BorderType::TopLeft));

    mNeighborDirs.push_back(glm::vec2( 0.0, 1.0));    //top
    mNeighborDirs.push_back(glm::vec2( 1.0, 1.0));    //topright
    mNeighborDirs.push_back(glm::vec2( 1.0, 0.0));    //right
    mNeighborDirs.push_back(glm::vec2( 1.0,-1.0));    //bottomright
    mNeighborDirs.push_back(glm::vec2( 0.0,-1.0));    //bottom
    mNeighborDirs.push_back(glm::vec2(-1.0,-1.0));    //bottomleft
    mNeighborDirs.push_back(glm::vec2(-1.0, 0.0));    //left
    mNeighborDirs.push_back(glm::vec2(-1.0, 1.0));    //topleft
}

BorderType::Type BorderType::operator()( int32_t Id ) const
{
    IdToBorderTypeMap_t::left_const_iterator i=mIdToBorderTypeMap.left.find(Id);
    BOOST_ASSERT(i!=mIdToBorderTypeMap.left.end());
    return (i!=mIdToBorderTypeMap.left.end())?i->second:BorderType::Top;
}

int32_t BorderType::operator()( Type type ) const
{
    IdToBorderTypeMap_t::right_const_iterator i=mIdToBorderTypeMap.right.find(type);
    BOOST_ASSERT(i!=mIdToBorderTypeMap.right.end());
    return (i!=mIdToBorderTypeMap.right.end())?i->second:BorderType::Top;
}

BorderType::NeighborDirs_t const& BorderType::GetNeighborDirs() const
{
    return mNeighborDirs;
}

