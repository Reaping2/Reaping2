#include "platform/i_platform.h"
#include "render/recognizer_factory.h"
#include "render/idle_recognizer.h"
#include "render/body_move_recognizer.h"
#include "render/leg_move_recognizer.h"
#include "render/death_recognizer.h"
#include "render/weapon_recognizer.h"
#include "render/pickup_recognizer.h"
#include "render/hat_recognizer.h"
#include "ctf_hat_recognizer.h"
#include "ctf_flag_recognizer.h"
#include "border_recognizer.h"
#include "head_recognizer.h"
namespace render {
RecognizerFactory::RecognizerFactory()
{
    Bind<DefaultRecognizer>( AutoId( "default_recognizer" ) );
    SetDefault( AutoId( "default_recognizer" ) );

    Bind<IdleRecognizer>( AutoId( "idle" ) );
    Bind<BodyMoveRecognizer>( AutoId( "body_move" ) );
    Bind<LegMoveRecognizer>( AutoId( "leg_move" ) );
    Bind<LegMoveRecognizer>( AutoId( "move" ) );
    Bind<DeathRecognizer>( AutoId( "death" ) );
    Bind<WeaponRecognizer>( AutoId( "weapon" ) );
    Bind<PickupRecognizer>( AutoId( "pickup" ) );
    Bind<HatRecognizer>( AutoId( "body_color" ) );
    Bind<ctf::CtfHatRecognizer>(AutoId("ctf_body_color"));
    Bind<ctf::CtfFlagRecognizer>(AutoId("flag"));
    Bind<BorderRecognizer>(AutoId("border"));
    Bind<HeadRecognizer>(AutoId("head"));
}
} // namespace render
