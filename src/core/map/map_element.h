#ifndef INCLUDED_CORE_MAP_MAP_ELEMENT_H
#define INCLUDED_CORE_MAP_MAP_ELEMENT_H

#include "platform/auto_id.h"
#include "core/opt.h"
#include "platform/jsonreader.h"
namespace map {

#define DEFINE_MAP_ELEMENT_BASE( BuffType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #BuffType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return BuffType::GetType_static(); \
    } \
    BuffType() = default; \
    virtual BuffType* clone() const \
    { \
        return new BuffType( *this ); \
    } \
 
class MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( MapElement );
    virtual ~MapElement();
    virtual void Load( Json::Value const& setters );
    virtual void Save( Json::Value& Element );
    int32_t GetIdentifier();
    void SetIdentifier( int32_t uId );
    void SetSpawnedActorGUID( int32_t spawnedActorGUID );   // mostly editor uses this information
    int32_t GetSpawnedActorGUID()const;                     // mostly editor uses this information
    int32_t GetUID() const;
    MapElement& operator=( MapElement const& other );
protected:
    MapElement( int32_t Id );

    void SetNextUID();

    int32_t mIdentifier;  //does not need to be unique. Some elements does not need UID. Some elements can have the same UID.
    int32_t mId;
    int32_t mSpawnedActorGUID;
    int32_t mUID;
};


class DefaultMapElement : public MapElement
{
public:
    DEFINE_MAP_ELEMENT_BASE( DefaultMapElement )
    DefaultMapElement( int32_t Id );
};
} // namespace map
#endif//INCLUDED_CORE_MAP_MAP_ELEMENT_H
