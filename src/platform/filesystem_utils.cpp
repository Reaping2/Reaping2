#include "filesystem_utils.h"
#include "platform/i_platform.h"

namespace fs_utils {

void for_each( std::string const& path, std::string const& extension, DescFunc_t&& func )
{
    fs_utils::for_each( path, extension, [&]( Json::Value const& desc, boost::filesystem::path const& path )
    {
        func( desc );
    } );
}


void for_each( std::string const& path, std::string const& extension, DescPathFunc_t&& func )
{
    using namespace platform;
    PathVect_t paths;
    auto& mFSys = Filesys::Get();
    mFSys.GetFileNames( paths, path );
    for (auto const& path : paths)
    {
        if (path.extension().string() != extension)
        {
            continue;
        }
        auto jsonFile = mFSys.Open( path );
        if (!jsonFile.get())
        {
            continue;
        }
        JsonReader reader( *jsonFile );
        if (!reader.IsValid())
        {
            continue;
        }
        auto const& Root = reader.GetRoot();
        if (!Root.isArray())
        {
            continue;
        }
        for (auto const& desc : Root)
        {
            try
            {
                func( desc, path );
            }
            catch (std::exception const& err)
            {
                L1( "Exception caught while parsing %s | *%s : %s", path.c_str(), extension.c_str(), err.what() );
            }
        }
    }
}

} // namespace fs_utils


