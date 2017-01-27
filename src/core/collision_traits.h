#ifndef INCLUDED_CORE_COLLISION_TRAITS_H
#define INCLUDED_CORE_COLLISION_TRAITS_H

#include "core/actor.h"
#include "core/collision_class.h"

namespace core {
struct CollisionTraits {
    static size_t constexpr NumClasses = CollisionClass::Num_Classes;
    static bool AreCorrelated( int32_t CC1, int32_t CC2 );
    static int32_t GetTrait( Actor const& actor );
    static double GetRadius( Actor const& actor );
};
}

#endif // INCLUDED_CORE_COLLISION_TRAITS_H
