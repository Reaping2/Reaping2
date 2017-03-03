#ifndef INCLUDED__BORDER_TYPE_H
#define INCLUDED__BORDER_TYPE_H

#include "platform/i_platform.h"
#include "boost/bimap.hpp"

class BorderType : public platform::Singleton<BorderType>
{
protected:
    friend class platform::Singleton<BorderType>;
    BorderType();
public:
    enum Type : int32_t
    {
        Top = 0,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,
        Num_Classes
    };
    BorderType::Type operator()( int32_t Id ) const;
    int32_t operator()( BorderType::Type type ) const;

    typedef std::vector<glm::vec2> NeighborDirs_t;
    NeighborDirs_t const& GetNeighborDirs() const;
private:
    NeighborDirs_t mNeighborDirs;
    typedef boost::bimap<int32_t, BorderType::Type> IdToBorderTypeMap_t;
    IdToBorderTypeMap_t mIdToBorderTypeMap;
};

#endif//INCLUDED__BORDER_TYPE_H


//command:  "classgenerator.exe" -g "enum" -c "border_type" -m "Top-top Right-right Bottom-bottom Left-left TopRight-topright BottomRight-bottomright BottomLeft-bottomleft TopLeft-topleft"
