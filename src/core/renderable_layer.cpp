#include "core/renderable_layer.h"


using platform::AutoId;

RenderableLayer::RenderableLayer()
{
    Init();
}

int32_t RenderableLayer::operator()( std::string const& Name ) const
{
    NameToPriority_t::const_iterator i = mNameToPriority.find( Name );
    BOOST_ASSERT( i != mNameToPriority.end() );
    return ( i != mNameToPriority.end() ) ? i->second : 0;
}

void RenderableLayer::Init()
{
    Filesys& FSys( Filesys::Get() );
    AutoFile F = FSys.Open( boost::filesystem::path( "misc/renderable_layers.json" ) );
    if (!F.get() || !F->IsValid())
    {
        return;
    }
    JsonReader Reader( *F );
    if (!Reader.IsValid())
    {
        return;
    }
    auto& Root = Reader.GetRoot();
    if (!Root.isArray())
    {
        return;
    }
    mNextPrio = 0;
    for (auto& desc : Root)
    {
        try
        {
            AddLayerFromOneDesc( desc );
        }
        catch (std::exception const& err)
        {
            L1( "Exception caught while parsing %s", err.what() );
        }
    }

}

void RenderableLayer::AddLayerFromOneDesc( Json::Value const& Desc )
{
    std::string layerName;
    if (!Json::GetStr( Desc["name"], layerName ))
    {
        return;
    }
    mNameToPriority[layerName] = mNextPrio++;
}

RenderableLayer::NameToPriority_t const& RenderableLayer::GetNameToPriorityMap() const
{
    return mNameToPriority;
}
