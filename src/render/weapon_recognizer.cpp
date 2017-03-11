#include "render/weapon_recognizer.h"
#include "core/i_inventory_component.h"
namespace render {

WeaponRecognizer::WeaponRecognizer( int32_t Id )
    : Recognizer( Id )
{

}

bool WeaponRecognizer::Recognize( Actor const& actor ) const
{
    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();
    if ( !inventoryC.IsValid() )
    {
        return false;
    }
    if ( inventoryC->GetSelectedItem( ItemType::Weapon ).IsValid() )
    {
        //L1( " weapon RECOGNIZED! \n" );
        return true;
    }
    return false;
}

} // namespace render
