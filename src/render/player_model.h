#ifndef INCLUDED_RENDERER_PLAYER_MODEL_H
#define INCLUDED_RENDERER_PLAYER_MODEL_H

class PlayerModel : public Model
{
	friend class ModelRepo;
	SpriteCollection const& mSprites;
	PlayerModel();
	void DrawPhase(SpritePhase const& Phase) const;
public:
	virtual void Draw(Actor const& Object)const;
};

#endif//INCLUDED_RENDERER_PLAYER_MODEL_H
