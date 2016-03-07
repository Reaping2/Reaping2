#ifndef INCLUDED_RENDER_ACTION_RENDERER_FACTORY_H
#define INCLUDED_RENDER_ACTION_RENDERER_FACTORY_H

#include "platform/factory.h"
#include "platform/singleton.h"
#include "render/action_renderer.h"
namespace render {
class ActionRendererFactory : public platform::Factory<ActionRenderer>, public platform::Singleton<ActionRendererFactory>
{
    friend class platform::Singleton<ActionRendererFactory>;
    ActionRendererFactory();
};
} // namespace render


#endif//INCLUDED_RENDER_ACTION_RENDERER_FACTORY_H