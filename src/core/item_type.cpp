#include "core/item_type.h"
#include "platform/auto_id.h"
#include "platform/settings.h"

using platform::AutoId;

glm::vec4 const ItemColorRepo::mDefaultColor = glm::vec4( 1, 1, 1, 1 );

ItemColorRepo::ItemColorRepo()
    : Repository<glm::vec4>( mDefaultColor )
{
    auto& settings = Settings::Get();
    int32_t i = ItemType::Normal;
    glm::vec4 col;
    col = settings.GetColor( "item_color.normal", glm::vec4( 0, 0, 1, 1 ) );
    mElements.insert( i, new glm::vec4(col) );
    i = ItemType::Weapon;
    col = settings.GetColor( "item_color.weapon", glm::vec4( 1, 1, 1, 1 ) );
    mElements.insert( i, new glm::vec4( col ) );
    i = ItemType::Buff;
    col = settings.GetColor( "item_color.buff", glm::vec4( 0, 1, 0, 1 ) );
    mElements.insert( i, new glm::vec4( col ) );
}

ItemType::ItemType()
{
    mIdToItemTypeMap.insert( IdToItemTypeMap_t::value_type( AutoId( "normal" ), ItemType::Normal ) );
    mIdToItemTypeMap.insert( IdToItemTypeMap_t::value_type( AutoId( "weapon" ), ItemType::Weapon ) );
    mIdToItemTypeMap.insert( IdToItemTypeMap_t::value_type( AutoId( "buff" ), ItemType::Buff ) );
}

ItemType::Type ItemType::operator()( int32_t Id ) const
{
    IdToItemTypeMap_t::left_const_iterator i = mIdToItemTypeMap.left.find( Id );
    BOOST_ASSERT( i != mIdToItemTypeMap.left.end() );
    return ( i != mIdToItemTypeMap.left.end() ) ? i->second : ItemType::Normal;
}

int32_t ItemType::operator()( Type type ) const
{
    IdToItemTypeMap_t::right_const_iterator i = mIdToItemTypeMap.right.find( type );
    BOOST_ASSERT( i != mIdToItemTypeMap.right.end() );
    return ( i != mIdToItemTypeMap.right.end() ) ? i->second : AutoId( "normal" );
}

