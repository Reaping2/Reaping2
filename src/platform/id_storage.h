#ifndef INCLUDED_CORE_ID_STORAGE_H
#define INCLUDED_CORE_ID_STORAGE_H

class IdStorage : public Singleton<IdStorage>
{
    static const int32_t mReservedIds;
    friend class Singleton<IdStorage>;
    IdStorage();
    int32_t mNextId;
    typedef std::map<std::string, int32_t> IdMap_t;
    IdMap_t mIdMap;
    // todo: egy sync method ehhez, network startupra
public:
    int32_t GetId( const std::string& Name );
    bool GetName( int32_t Id, std::string& Name )const;
};

#endif//INCLUDED_CORE_ID_STORAGE_H
