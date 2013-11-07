#include"i_core.h"

ActionRepo::ActionRepo()
	:Repository(DefaultAction::Get())
{	// ittene majd szepen beolvasva adatfajlbol a parameterezeseket (idotartam, sebesseg, sebzes, spawned creature name, fenetudja)
	Action* Act=new MoveAction(); int32_t Id=Act->GetId();
	mElements.insert(Id,Act);
	Act=new ShootAction(); Id=Act->GetId();
	mElements.insert(Id,Act);
	Act=new BodyMoveAction(); Id=Act->GetId();
	mElements.insert(Id,Act);
	Act=new BodyIdleAction(); Id=Act->GetId();
	mElements.insert(Id,Act);
	Act=new IdleAction(); Id=Act->GetId();
	mElements.insert(Id,Act);
}
