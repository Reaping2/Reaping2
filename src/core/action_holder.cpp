#include"i_core.h"

ActionHolder::ActionHolder()
{	// ittene majd szepen beolvasva adatfajlbol a parameterezeseket (idotartam, sebesseg, sebzes, spawned creature name, fenetudja)
	Action* Act=new MoveAction(); int32_t Id=Act->GetId();
	mActions.insert(Id,Act);
	Act=new ShootAction(); Id=Act->GetId();
	mActions.insert(Id,Act);
}

const Action * ActionHolder::GetAction(int32_t What) const
{
	ActionMap_t::const_iterator i=mActions.find(What);
	return i==mActions.end()?(NULL):(i->second);
}

const Action * ActionHolder::GetAction(std::string const& What) const
{
	return GetAction(IdStorage::Get().GetId(What));
}
