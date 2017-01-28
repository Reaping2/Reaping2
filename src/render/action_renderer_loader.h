#ifndef INCLUDED_RENDER_ACTION_RENDERER_LOADER_H
#define INCLUDED_RENDER_ACTION_RENDERER_LOADER_H
#include "core/property_loader.h"

namespace render {
    class ActionRenderer;
}

namespace render {
template<typename ACTION_RENDERER>
class ActionRendererLoader : public PropertyLoader<ACTION_RENDERER, render::ActionRenderer>
{
};

}
#endif//INCLUDED_RENDER_ACTION_RENDERER_LOADER_H
