#ifndef INCLUDED_RENDERER_PLAYER_MODEL_H
#define INCLUDED_RENDERER_PLAYER_MODEL_H

class PlayerModel : public Model
{
	friend class ModelRepo;
	PlayerModel();
public:
	virtual void Draw(Actor const& Object)const;
};

#endif//INCLUDED_RENDERER_PLAYER_MODEL_H
