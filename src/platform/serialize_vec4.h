#ifndef INCLUDED_PLATFORM_SERIALIZE_VEC4_H
#define INCLUDED_PLATFORM_SERIALIZE_VEC4_H

#include "platform/i_platform.h"

namespace boost {
    namespace serialization {

        template<class Archive>
        void serialize( Archive & ar, glm::vec4 & g, const unsigned int version )
        {
            ar & g.x;
            ar & g.y;
            ar & g.z;
            ar & g.w;
        }

    } // namespace serialization
} // namespace boost

#endif // INCLUDED_PLATFORM_SERIALIZE_VEC4_H
