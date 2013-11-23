#include "i_core.h"

void CollisionStore::SetCollisionModel( CollisionClass::Type T1, CollisionClass::Type T2,Elem_t Model )
{
	size_t Id=MapTypes(T1,T2);
	mModels[Id]=Model;
	Id=MapTypes(T2,T1);
	mModels[Id]=Model;
}

CollisionModel const& CollisionStore::GetCollisionModel( CollisionClass::Type T1, CollisionClass::Type T2 ) const
{
	return *(mModels.at(MapTypes(T1,T2)));
}

CollisionStore::CollisionStore()
{
	Elem_t Empty(new EmptyCollisionModel);
	size_t Size=MapTypes(CollisionClass::Num_Classes,CollisionClass::Num_Classes);
	mModels.resize(Size,Empty);
	Elem_t Box(new BoxCollisionModel);
	SetCollisionModel(CollisionClass::Creep,CollisionClass::Projectile,Box);
}

size_t CollisionStore::MapTypes( CollisionClass::Type T1, CollisionClass::Type T2 )
{
	return size_t(T1)*CollisionClass::Num_Classes+T2;
}
