#ifndef INCLUDED__RUSTY_REAPER_H
#define INCLUDED__RUSTY_REAPER_H

#include "core/weapon.h"
#include "platform/export.h"

class RustyReaper : public Weapon
{
public:
    RustyReaper( int32_t Id );
    RustyReaper();
    void SetSawGUID( int32_t sawGUID );
    int32_t GetSawGUID()const;
    void SetSawActive( bool sawActive );
    bool IsSawActive()const;
private:
    int32_t mSawGUID;
    bool mSawActive;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void RustyReaper::serialize(Archive& ar, const unsigned int version)
{
    ar& boost::serialization::base_object<Weapon>(*this);
    ar& mSawGUID;
    ar& mSawActive;
}

class RustyReaperLoader : public ItemLoader<RustyReaper>
{
public:
    virtual void BindValues();
    RustyReaperLoader();
    friend class ItemLoaderFactory;
};

REAPING2_CLASS_EXPORT_KEY2( RustyReaper, RustyReaper, "rusty_reaper" );

#endif//INCLUDED__RUSTY_REAPER_H

//command:  "classgenerator.exe" -g "weapon" -c "rusty_reaper"
