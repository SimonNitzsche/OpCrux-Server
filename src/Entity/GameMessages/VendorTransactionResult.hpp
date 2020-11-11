#ifndef __ENTITY__GM__VendorTransactionResult_HPP__
#define __ENTITY__GM__VendorTransactionResult_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct VendorTransactionResult : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_VENDOR_TRANSACTION_RESULT;
		}

		std::int32_t iResult;

		VendorTransactionResult() : GMBase() {}
		void Deserialize(RakNet::BitStream* bs) {

		}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, iResult);
		}

	};
}

#endif