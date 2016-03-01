#pragma once
#ifndef INCLUDED_STATIC_INIT_H
#define INCLUDED_STATIC_INIT_H

#define FORCE_STATIC_INIT( x ) \
    namespace ns_ ## x ## _force_init { \
        bool forceInit ## x (); \
        namespace { \
            volatile bool force_init_ ## x = forceInit ## x (); \
        } \
    }

#define FORCE_STATIC_INIT_IMPLEMENT( x ) \
    namespace ns_ ## x ## _force_init { \
        bool forceInit ## x () { volatile bool rv = true; return rv ; } \
    }

// FORCE_STATIC_INIT usage:
// put FORCE_STATIC_INIT( UniqueID ) in header
// put FORCE_STATIC_INIT_IMPLEMENT( UniqueID ) in cpp to be force-static inited
// include header from main.cpp, or a sure-to-be static inited cpp
// yeah, ugly, but i don't know other cross-platform solutions
// and -Wl,whole-archive includes shitloads of extra data, even if not needed
// this solution allows a bit more selectiveness

#endif // INCLUDED_STATIC_INIT_H

