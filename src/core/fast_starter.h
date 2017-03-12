#ifndef INCLUDED_CORE_FAST_STARTER_H
#define INCLUDED_CORE_FAST_STARTER_H
#include <string>
#include "platform/i_platform.h"

namespace core {

    class FastStarter
    {
    public:
        FastStarter( std::string const& shortcut );
        ModelValue mMainModel;
        ModelValue mSleepModel;
        void Sleep( int32_t millis );
    };

} // namespace core
#endif//INCLUDED_CORE_FAST_STARTER_H
