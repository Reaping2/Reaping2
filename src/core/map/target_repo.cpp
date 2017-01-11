#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "../ctf_program_state.h"
#include "pickup_target.h"
#include "flag_spawn_target.h"
#include "soldier_spawn_target.h"

using platform::AutoId;

namespace map {

DefaultTarget const TargetRepo::mDefault = DefaultTarget(-1);

TargetRepo::TargetRepo()
    : Repository<ITarget>( mDefault )
    , mTargetFactory( TargetFactory::Get() )
{
    Init();

}

void TargetRepo::Init()
{
    using boost::filesystem::path;
    Filesys& FSys = Filesys::Get();
    std::vector<boost::filesystem::path> paths;
    FSys.GetFileNames(paths, "editor_targets" );
    for ( auto const& path : paths )
    {
        if (path.extension().string() != ".json")
        {
            continue;
        }
        AutoFile JsonFile = FSys.Open( path );
        if ( !JsonFile.get() )
        {
            L1("Cannot open %\n", path.string().c_str());
            continue;
        }
        JsonReader Reader( *JsonFile );
        if ( !Reader.IsValid() )
        {
            L1("Invalid JSON file: %s\n", path.string().c_str());
            continue;
        }
        Json::Value Root = Reader.GetRoot();
        if (!Root.isObject())
        {
            L1("%s does not contian JSON object as root element", path.string().c_str());
            continue;
        }

        // initialize target from the JSON file
        try
        {
            std::string target_name;
            if (!Json::GetStr(Root["target_name"], target_name ))
            {
                continue;
            }
            std::string name;
            if (!Json::GetStr(Root["name"], name ))
            {
                continue;
            }
            int32_t target_autoid = AutoId(target_name);
            int32_t autoid = AutoId( name );
            auto target = mTargetFactory( target_autoid );
            const Json::Value& setters = Root["setters"];
            if ( target->Load(setters) )
            {
                mElements.insert( autoid , target.release());
            }
        }
        catch( const std::exception& e )
        {
            L1("Exception caught while initializing targets: %s\n", e.what());
            continue;
        }
    }
}


void DefaultTarget::Update( double DeltaTime )
{

}

DefaultTarget::DefaultTarget(int32_t)
    : ITarget( -1 )
{

}

void DefaultTarget::PutTarget( glm::vec2 position )
{

}

bool DefaultTarget::Load( const Json::Value& )
{
    return true;
}

} // namespace map

