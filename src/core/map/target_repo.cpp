#include "platform/i_platform.h"
#include "target_repo.h"
#include "wall_target.h"
#include "ctf_soldier_spawn_target.h"
#include "../ctf_program_state.h"
#include "pickup_target.h"
#include "flag_spawn_target.h"
#include "soldier_spawn_target.h"
#include "platform/filesystem_utils.h"

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
    fs_utils::for_each( "actors", ".target", [&]( Json::Value const& desc )
    {
        std::string target_name;
        if (!Json::GetStr( desc["target_name"], target_name ))
        {
            return;
        }
        std::string name;
        if (!Json::GetStr( desc["name"], name ))
        {
            return;
        }
        int32_t target_autoid = AutoId( target_name );
        int32_t autoid = AutoId( name );
        auto target = mTargetFactory( target_autoid );
        const Json::Value& setters = desc["setters"];
        if (target->Load( setters ))
        {
            mElements.insert( autoid, target.release() );
        }
    } );
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

