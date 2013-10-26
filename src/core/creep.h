#ifndef INCLUDED_CORE_CREEP_H
#define INCLUDED_CORE_CREEP_H

class Creep : public Actor
{
public:
	Creep(double x=0., double y=0.);
	Creep(Actor* player,double x=0., double y=0.);
};

#endif//INCLUDED_CORE_CREEP_H
