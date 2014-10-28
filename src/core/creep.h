#ifndef INCLUDED_CORE_CREEP_H
#define INCLUDED_CORE_CREEP_H

class Creep : public Actor
{
    virtual void OnDeath();
public:
    Creep( std::string const& Name, double x = 0., double y = 0., Actor* player = NULL );
};

#endif//INCLUDED_CORE_CREEP_H
