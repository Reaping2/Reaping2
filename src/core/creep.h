#ifndef INCLUDED_CORE_CREEP_H
#define INCLUDED_CORE_CREEP_H

class Creep : public Actor
{
public:
    Creep( std::string const& Name, double x = 0., double y = 0., Actor* player = NULL );
    virtual void Update( double Seconds );
private:
    bool mTriedDrop;
};

#endif//INCLUDED_CORE_CREEP_H
