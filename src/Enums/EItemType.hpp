#ifndef __ENUMS__EITEMTYPE_HPP__
#define __ENUMS__EITEMTYPE_HPP__

#include <memory>

namespace Enums {
    enum class EItemType : std::int32_t {
        Invalid,
        Brick,
        Hat,
        Hair,
        Neck,
        LeftHand,
        RightHand,
        Legs,
        LeftTrinket,
        RightTrinket,
        Behavior,
        Property,
        Model,
        Collectable,
        Consumable,
        Chest,
        Egg,
        PetFood,
        QuestObject,
        PetInventoryItem,
        T20Package,
        LootModel,
        Vehicle,
        Currency
    };
}

#endif