#ifndef INCLUDED_CORE_WALL_H
#define INCLUDED_CORE_WALL_H

class Wall : public Actor
{
public:
    Wall( std::string const& Name );
    void Collide( Actor& Other );
};

#endif//INCLUDED_CORE_WALL_H
