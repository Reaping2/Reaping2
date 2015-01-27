#ifndef INCLUDED_RENDER_WEAPON_RECOGNIZER_H
#define INCLUDED_RENDER_WEAPON_RECOGNIZER_H
#include "platform/i_platform.h"
#include "render/recognizer.h"
namespace render {

    class WeaponRecognizer: public Recognizer
    {
    public:
        WeaponRecognizer( int32_t Id );
        virtual bool Recognize(Actor const&);
    };

} // namespace render

#endif //INCLUDED_RENDER_WEAPON_RECOGNIZER_H