#include "i_render.h"

//todo: z order from sprite json?

namespace{
	struct ActionOrder
	{
		ActionOrder()
		{	// force autoid order
			AutoId("idle");
			AutoId("move");
			AutoId("body_idle");
			AutoId("body_move");
		}
	}ActonOrderInst;
}
