#ifndef INCLUDED_PLATFORM_TGATEXTURE_H
#define INCLUDED_PLATFORM_TGATEXTURE_H

#include "texture_base.h"
#include "rstdint.h"

namespace platform {
class File;

class TgaTexture : public TextureBase
{
public:
    TgaTexture( File& F );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_TGATEXTURE_H
