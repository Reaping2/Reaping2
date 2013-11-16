#include"i_core.h"

ActionRepo::ActionRepo()
	:mFactory(Factory<Action, Actor>::Get())
{	
	mFactory.Bind<DefaultAction>(AutoId("default_action"));
	mFactory.SetDefault(AutoId("default_action"));

	mFactory.Bind<MoveAction>(AutoId("move"));
	mFactory.Bind<ShootAction>(AutoId("shoot"));
	mFactory.Bind<ShootAltAction>(AutoId("shoot_alt"));
	mFactory.Bind<BodyMoveAction>(AutoId("body_move"));
	mFactory.Bind<BodyIdleAction>(AutoId("body_idle"));
	mFactory.Bind<IdleAction>(AutoId("idle"));
	mFactory.Bind<PlasmaGunAction>(AutoId("plasma_gun"));
}
