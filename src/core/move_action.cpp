#include "i_core.h"

MoveAction::MoveAction()
	:Action()
{
	mBlocksActions|=1<<ActionHolder::SHOOT;
	mCancelsActions|=1<<ActionHolder::SHOOT;
}

