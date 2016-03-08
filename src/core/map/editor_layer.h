#ifndef INCLUDED_MAP_EDITOR_LAYER_H
#define INCLUDED_MAP_EDITOR_LAYER_H

#include "platform/singleton.h"
#include "boost/bimap.hpp"

namespace map {

class EditorLayer : public platform::Singleton<EditorLayer>
{
protected:
    friend class platform::Singleton<EditorLayer>;
    EditorLayer();
public:
    enum Type
    {
        Any = 0,
        Target,
        Num_Classes
    };
    EditorLayer::Type operator()( int32_t Id ) const;
    int32_t operator()( EditorLayer::Type type ) const;
private:
    typedef boost::bimap<int32_t, EditorLayer::Type> IdToEditorLayerMap_t;
    IdToEditorLayerMap_t mIdToEditorLayerMap;
};

} // namespace map

#endif//INCLUDED_MAP_EDITOR_LAYER_H


//command:  "classgenerator.exe" -g "enum" -m "Any-any Target-target" -c "editor_layer" -n "map"
