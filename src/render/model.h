#ifndef INCLUDED_RENDERER_MODEL_H
#define INCLUDED_RENDERER_MODEL_H

class Actor;
class Model
{
public:
	virtual ~Model(){}
	virtual void Draw(Actor const& Object)const=0;
};

#endif//INCLUDED_RENDERER_MODEL_H
