#ifndef INCLUDED_CORE_AUTO_ID_H
#define INCLUDED_CORE_AUTO_ID_H
class AutoId
{
public:
    AutoId( std::string const& Name );
    int32_t GetId()const;
    virtual ~AutoId();
    operator int32_t()const;
protected:
    const int32_t mId;
};


#endif//INCLUDED_CORE_AUTO_ID_H