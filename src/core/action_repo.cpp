#include"i_core.h"

ActionRepo::ActionRepo()
{	// ittene majd szepen beolvasva adatfajlbol a parameterezeseket (idotartam, sebesseg, sebzes, spawned creature name, fenetudja)
	mDefaultElement=boost::bind(&ActionRepo::Create<DefaultAction>,_1,_2);

	mElements[AutoId("move")]=boost::bind(&ActionRepo::Create<MoveAction>,_1,_2);
	mElements[AutoId("shoot")]=boost::bind(&ActionRepo::Create<ShootAction>,_1,_2);
	mElements[AutoId("shoot_alt")]=boost::bind(&ActionRepo::Create<ShootAltAction>,_1,_2);
	mElements[AutoId("body_move")]=boost::bind(&ActionRepo::Create<BodyMoveAction>,_1,_2);
	mElements[AutoId("body_idle")]=boost::bind(&ActionRepo::Create<BodyIdleAction>,_1,_2);
	mElements[AutoId("idle")]=boost::bind(&ActionRepo::Create<IdleAction>,_1,_2);
	mElements[AutoId("plasma_gun")]=boost::bind(&ActionRepo::Create<PlasmaGunAction>,_1,_2);
}
Action * ActionRepo::operator()( int32_t Id, Actor& actor )
{
	return ((const ActionRepo*)this)->operator ()(Id,actor);
}


Action * ActionRepo::operator()( int32_t Id, Actor& actor ) const
{
	ElementMap_t::const_iterator i=mElements.find(Id);
	return (i==mElements.end()?mDefaultElement(Id,actor):(i->second)(Id,actor));
}

void ActionRepo::Register(int32_t Id, ActionFunctor functor)
{
	mElements[Id]=functor;
}
// ez mennyire gyilkosmar? :D
template<typename Element_T>
Action * ActionRepo::Create(int32_t Id, Actor& actor)
{
	return new Element_T(Id,actor);
}