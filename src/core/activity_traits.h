#ifndef INCLUDED_CORE_ACTIVITY_TRAITS_H
#define INCLUDED_CORE_ACTIVITY_TRAITS_H

#include "core/actor.h"
#include <functional>

namespace core {
struct ActivityTraits {
    enum Types {
        Active = 0,
        Passive,
        NumTypes
    };
    static size_t constexpr NumClasses = NumTypes;
    static bool AreCorrelated( int32_t CC1, int32_t CC2 );
    static int32_t GetTrait( Actor const& actor );
    static double GetRadius( Actor const& actor );
    typedef std::function<float(int32_t)> ActorScaleFunc;
    static void SetActorScaleFunc( ActorScaleFunc const& func );
    typedef std::function<double(void)> ActiveSizeFunc;
    static void SetActiveSizeFunc( ActiveSizeFunc const& func );
private:
    static ActorScaleFunc mScaleFunc;
    static ActiveSizeFunc mActiveSize;
};
}

#endif // INCLUDED_CORE_ACTIVITY_TRAITS_H
