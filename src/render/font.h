#ifndef INCLUDED_RENDER_FONT_H
#define INCLUDED_RENDER_FONT_H

class Font : public Singleton<Font>
{
	friend class Singleton<Font>;
	Font();

	char mFirstChar;
	char mLastChar;
	GLuint mTexId;
	GLfloat mMaxHeight;
	static SpritePhase DefaultSprite;
	typedef std::vector<SpritePhase> Characters_t;
	Characters_t mChars;
	struct CharDesc
	{
		char Code;
		SpritePhase Phase;
	};
	static bool LoadChar(CharDesc& Character,Texture const& Tex,Json::Value& CharValue);
	void Load(std::string const& Path);
public:
	glm::vec2 GetDim(std::string const& Text)const;
	GLfloat GetFontSize()const;
//	void Draw(std::string const& Text)const;
	SpritePhase const& GetChar(char C) const;
	GLuint GetTexId()const {return mTexId;}
};

#endif//INCLUDED_RENDER_FONT_H
