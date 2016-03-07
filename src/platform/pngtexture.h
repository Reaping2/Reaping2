#ifndef INCLUDED_PLATFORM_PNGTEXTURE_H
#define INCLUDED_PLATFORM_PNGTEXTURE_H

#include "texture_base.h"

#include <memory>

namespace platform {
class File;

class PngTexture : public TextureBase
{
public:
    PngTexture( File& F );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_PNGTEXTURE_H
