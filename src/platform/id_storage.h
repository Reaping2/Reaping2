#ifndef INCLUDED_CORE_ID_STORAGE_H
#define INCLUDED_CORE_ID_STORAGE_H

#include "singleton.h"
#include "stdint.h"
#include <string>
#include <memory>

namespace platform {
namespace detail {
class IdStorageImpl;
} // namespace detail

class IdStorage : public Singleton<IdStorage>
{
    friend class Singleton<IdStorage>;
    IdStorage();
    std::auto_ptr< detail::IdStorageImpl > mImpl;
    // todo: serialize / deserialize
public:
    ~IdStorage();
    int32_t GetId( const std::string& Name );
    bool GetName( int32_t Id, std::string& Name )const;
};

} // namespace platform

#endif//INCLUDED_CORE_ID_STORAGE_H
