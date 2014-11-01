#ifndef INCLUDED_PLATFORM_TGATEXTURE_H
#define INCLUDED_PLATFORM_TGATEXTURE_H

#include "texture_base.h"
#include <stdint.h>

namespace platform {
class File;

class TgaTexture : public TextureBase
{
public:
    TgaTexture( File& F );
    static void Reorder( uint8_t* Data, size_t Size );
private:
    void Load( File& F );
};

} // namespace platform

#endif//INCLUDED_PLATFORM_TGATEXTURE_H
