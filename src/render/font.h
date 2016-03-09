#ifndef INCLUDED_RENDER_FONT_H
#define INCLUDED_RENDER_FONT_H
#include "platform/i_platform.h"
#include "sprite_phase.h"
#include "texture.h"

struct Character
{
    SpritePhase Phase;
    glm::vec2 Size;
};

class Font : public Singleton<Font>
{
    friend class Singleton<Font>;
    Font();

    char mFirstChar;
    char mLastChar;
    GLuint mTexId;
    GLfloat mMaxHeight;
    Character mDefaultChar;
    typedef std::vector<Character> Characters_t;
    Characters_t mChars;
    struct CharDesc
    {
        char Code;
        Character Phase;
    };
    static bool LoadChar( CharDesc& Character, Texture const& Tex, Json::Value& CharValue );
    void Load( std::string const& Path );
public:
    glm::vec2 GetDim( std::string const& Text )const;
    GLfloat GetFontSize()const;
    //  void Draw(std::string const& Text)const;
    Character const& GetChar( char C ) const;
    GLuint GetTexId()const
    {
        return mTexId;
    }
};

#endif//INCLUDED_RENDER_FONT_H
