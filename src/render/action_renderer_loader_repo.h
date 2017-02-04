#ifndef INCLUDED_RENDER_ACTION_RENDERER_LOADER_REPO_H
#define INCLUDED_RENDER_ACTION_RENDERER_LOADER_REPO_H

#include "platform/repository.h"
#include "platform/singleton.h"
#include "platform/i_platform.h"
#include "action_renderer.h"
#include "core/property_loader.h"

namespace render {
class ActionRendererLoaderRepo : public platform::Repository<PropertyLoaderBase<ActionRenderer>>, public platform::Singleton<ActionRendererLoaderRepo>
{
    friend class platform::Singleton<ActionRendererLoaderRepo>;
    static DefaultActionRendererLoader const mDefault;
    ActionRendererLoaderRepo();
    using platform::Repository<PropertyLoaderBase<ActionRenderer>>::operator();
public:
    PropertyLoaderBase<ActionRenderer> const& operator()( int32_t actorId, int32_t Id ) const;
    PropertyLoaderBase<ActionRenderer> const& operator()( int32_t actorId, int32_t Id );
};

} // namespace render

#endif//INCLUDED_RENDER_ACTION_RENDERER_LOADER_REPO_H

//command:  "classgenerator.exe" -g "repository" -n "render" -c "action_renderer_loader_repo" -t "action_renderer_loader"
