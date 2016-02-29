#include "platform/i_platform.h"
#include "item_loader_factory.h"
#include "property_loader.h"
#include "item.h"
#include "plasma_gun.h"

using platform::AutoId;

DefaultItemLoader const ItemLoaderRepo::mDefault = DefaultItemLoader();
ItemLoaderRepo::ItemLoaderRepo()
    : Repository<PropertyLoaderBase<Item> >(mDefault)
{
    int32_t id=AutoId("plasma_gun"); mElements.insert(id,/*std::auto_ptr<PropertyLoaderBase<Item>*/(new PlasmaGunLoader()));
    Init();
}

void ItemLoaderRepo::Init()
{
    PathVect_t Paths;
    Filesys& FSys = Filesys::Get();
    FSys.GetFileNames( Paths, "items" );
    for( PathVect_t::const_iterator i = Paths.begin(), e = Paths.end(); i != e; ++i )
    {
        boost::filesystem::path const& Path = *i;
        if( Path.extension().string() != ".json" )
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( *i );
        if( !JsonFile.get() )
        {
            continue;
        }
        JsonReader Reader( *JsonFile );
        if( !Reader.IsValid() )
        {
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if( !Root.isArray() )
        {
            continue;
        }
        for( Json::Value::iterator i = Root.begin(), e = Root.end(); i != e; ++i )
        {
            Json::Value& ItemDesc = *i;
            if( !LoadItemFromOneDesc( ItemDesc ) )
            {
                return;
            }
        }
    }

}

bool ItemLoaderRepo::LoadItemFromOneDesc(Json::Value& ItemDesc)
{
    std::string nameStr;
    if( !Json::GetStr( ItemDesc["name"], nameStr))
    {
        return false;
    }
    PropertyLoaderBase<Item>& itemLoader=operator()(AutoId(nameStr));

    Json::Value& setters=ItemDesc["set"];
    if (!setters.isArray())
    {
        return false;
    }
    if (setters.empty())
    {
        return true;
    }
    L1("Load item_loader: %s\n",nameStr.c_str());
    itemLoader.Load(*setters.begin());
    return true;
}

