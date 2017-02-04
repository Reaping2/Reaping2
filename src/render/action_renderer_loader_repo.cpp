#include "platform/i_platform.h"
#include "action_renderer_loader_repo.h"
#include "action_renderer.h"

using platform::AutoId;

namespace render {

DefaultActionRendererLoader const ActionRendererLoaderRepo::mDefault;

ActionRendererLoaderRepo::ActionRendererLoaderRepo()
    : Repository<PropertyLoaderBase<ActionRenderer>>(mDefault)
{
    // TODO: turned out this feature is not needed yet. although it could be useful in the future.
    // this class should read all the .render files and get the renderers. where they are an object like:
/*
    "recognizers":
    [
        {"recognizer":"move",
            { "renderer":"move", "set" : {"color":0xFF0000FF} }}
    ],
*/
    // it should load the corresponding renderer loader data the usual way.
    // this way a LoaderFactory is needed too for the loadable renderers.
}

PropertyLoaderBase<ActionRenderer> const& ActionRendererLoaderRepo::operator()( int32_t actorId, int32_t Id ) const
{
    return mDefault;
}

PropertyLoaderBase<ActionRenderer> const& ActionRendererLoaderRepo::operator()( int32_t actorId, int32_t Id )
{
    return const_cast<PropertyLoaderBase<ActionRenderer>&>(((const ActionRendererLoaderRepo*)this)->operator ()( actorId, Id ));
}

} // namespace render

