#include "texture_base.h"
#include "detail/TextureImpl.h"
#include <boost/assert.hpp>

namespace platform {

TextureBase::~TextureBase()
{
}

size_t TextureBase::GetWidth() const
{
    BOOST_ASSERT( IsValid() );
    return mImpl->mWidth;
}

size_t TextureBase::GetHeight() const
{
    BOOST_ASSERT( IsValid() );
    return mImpl->mHeight;
}

size_t TextureBase::GetChannels() const
{
    return detail::TextureImpl::mChannels;
}

uint8_t const* TextureBase::GetData() const
{
    BOOST_ASSERT( IsValid() );
    return &mImpl->mData.at(0);
}

bool TextureBase::IsValid() const
{
    return mImpl.get() != NULL;
}

} // namespace platform

