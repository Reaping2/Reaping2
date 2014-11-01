#ifndef INCLUDED_PLATFORM_TEXTURE_BASE_H
#define INCLUDED_PLATFORM_TEXTURE_BASE_H

#include <stdint.h>
#include <cstddef>
#include <memory>

namespace platform {
namespace detail {
class TextureImpl;
}

class TextureBase
{
public:
    virtual ~TextureBase();
    size_t GetWidth()const;
    size_t GetHeight()const;
    size_t GetChannels()const;
    uint8_t const* GetData()const;
    bool IsValid()const;
protected:
    std::auto_ptr< detail::TextureImpl > mImpl;

};

} // namespace platform

#endif//INCLUDED_PLATFORM_TEXTURE_BASE_H
