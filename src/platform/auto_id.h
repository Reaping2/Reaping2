#ifndef INCLUDED_CORE_AUTO_ID_H
#define INCLUDED_CORE_AUTO_ID_H

#include <string>
#include <stdint.h>

namespace platform {

class AutoId
{
public:
    AutoId( std::string const& Name );
    int32_t GetId()const;
    ~AutoId();
    operator int32_t()const;
protected:
    const int32_t mId;
};

} // namespace platform

#endif//INCLUDED_CORE_AUTO_ID_H
