#ifndef INCLUDED_PLATFORM_PNGTEXTURE_H
#define INCLUDED_PLATFORM_PNGTEXTURE_H

#include "texture_base.h"

#include <memory>

namespace platform {
class File;
namespace detail {
class PngLoadData;
}

class PngTexture : public TextureBase
{
public:
    PngTexture( File& F );
private:
    friend class detail::PngLoadData;
    std::auto_ptr< detail::PngLoadData > mLoadData;

};

} // namespace platform

#endif//INCLUDED_PLATFORM_PNGTEXTURE_H
