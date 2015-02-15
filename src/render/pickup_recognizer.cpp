#include "render/pickup_recognizer.h"
#include "core/pickup_collision_component.h"
namespace render {

    PickupRecognizer::PickupRecognizer(int32_t Id)
        : Recognizer(Id)
    {

    }

    bool PickupRecognizer::Recognize(Actor const& actor)
    {
        return actor.Get<PickupCollisionComponent>().IsValid();
    }

} // namespace render