#include "core/flag_receiver_component.h"
#include <portable_iarchive.hpp>
#include <portable_oarchive.hpp>

namespace ctf {

FlagReceiverComponent::FlagReceiverComponent()
{
}



void FlagReceiverComponentLoader::BindValues()
{
}

FlagReceiverComponentLoader::FlagReceiverComponentLoader()
{
}

} // namespace ctf

REAPING2_CLASS_EXPORT_IMPLEMENT( ctf__FlagReceiverComponent, ctf::FlagReceiverComponent );
