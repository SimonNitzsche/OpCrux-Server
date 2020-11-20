#ifndef __ENUMS__EINVENTORYTYPE_HPP__
#define __ENUMS__EINVENTORYTYPE_HPP__

#include <memory>

namespace Enums {
    enum class EInventoryType : std::int32_t {
        None = -1,
        Items,
        VaultItems,
        Bricks,
        TemporaryItems = 4,
        Models,
        TemporaryModels,
        Behaviors,
        PropertyDeeds,
        VendorBuyback = 11,
        Hidden = 12,
        BrickDonations,
        VaultModels = 14,
        Invalid = 100
    };
}

#endif