#include "core/flag_carrier_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace ctf {

FlagCarrierComponent::FlagCarrierComponent()
{
}



void FlagCarrierComponentLoader::BindValues()
{
}

FlagCarrierComponentLoader::FlagCarrierComponentLoader()
{
}

} // namespace ctf

REAPING2_CLASS_EXPORT_IMPLEMENT( ctf__FlagCarrierComponent, ctf::FlagCarrierComponent );
