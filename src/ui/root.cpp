#include "i_ui.h"

Root::Root()
    : Widget( AutoId( "root" ) )
{
    mDimensions = glm::vec4( 0., 0., 100., 100. );
    mDimSet = true;
}
