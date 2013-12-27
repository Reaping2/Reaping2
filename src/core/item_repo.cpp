#include "i_core.h"

ItemRepo::ItemRepo()
{
	Bind<DefaultItem>(AutoId("default_item"));
	SetDefault(AutoId("default_item"));
	Bind<PlasmaGunAction>(AutoId("plasma_gun"));
	Bind<Pistol>(AutoId("pistol"));
}
