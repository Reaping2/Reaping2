#include "core/map/editor_layer.h"
#include "platform/auto_id.h"

using platform::AutoId;

namespace map {

EditorLayer::EditorLayer()
{
    mIdToEditorLayerMap.insert( IdToEditorLayerMap_t::value_type( AutoId( "any" ), EditorLayer::Any ) );
    mIdToEditorLayerMap.insert( IdToEditorLayerMap_t::value_type( AutoId( "target" ), EditorLayer::Target ) );
}

EditorLayer::Type EditorLayer::operator()( int32_t Id ) const
{
    IdToEditorLayerMap_t::left_const_iterator i = mIdToEditorLayerMap.left.find( Id );
    BOOST_ASSERT( i != mIdToEditorLayerMap.left.end() );
    return ( i != mIdToEditorLayerMap.left.end() ) ? i->second : EditorLayer::Any;
}

int32_t EditorLayer::operator()( Type type ) const
{
    IdToEditorLayerMap_t::right_const_iterator i = mIdToEditorLayerMap.right.find( type );
    BOOST_ASSERT( i != mIdToEditorLayerMap.right.end() );
    return ( i != mIdToEditorLayerMap.right.end() ) ? i->second : EditorLayer::Any;
}

} // namespace map

