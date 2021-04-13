#pragma once
#ifndef _LWOOBJID_HPP__
#define _LWOOBJID_HPP__

#include <string>
#include <sstream>
#include "Utils/Logger.hpp"

namespace DataTypes {

	/*
		Each Object in LEGO Universe requires an unique object id (unsigned long long).
		This class is a wrapper to better control the object ids.
	*/
	class LWOOBJID {
		private:
			
			// The Flag Size. (Adjustable - make sure the type specifier fits it)
			static const unsigned char flagSize = 8;

			// The Id Size, calculated with flag size.
			static const unsigned char idSize = 64 - flagSize;

			// The Data, the object id.
			std::uint64_t data;

		public:

			/*
				Category in which is object id is used for.
			*/
			enum class CATEGORY {
				STATIC = 0, 
				LOCAL,
				GLOBAL,
				SPAWNED
			};

			/*
				Transforms LWOOBJID into unsigned long long.
			*/
			operator std::uint64_t() const {
				// Return data.
				return data;
			}
			
			/*
				Sets LWOOBJID.
			*/
			LWOOBJID & operator=(const std::uint64_t other) {
				// Set data to the received obj id.
				this->data = other;

				// If received obj id is = -1, set data to 0.
				if (other == -1) this->data = 0;

				// Return a pointer to this LWOOBJID.
				return *this;
			}

			/*
				Constructs a LWOOBJID without any specified obj id.
			*/
			LWOOBJID() { this->data = NULL; }

			/*
				Constructs a LWOOBJID with specified obj id.
			*/
			LWOOBJID(std::uint64_t unsll) {
				// Check if received obj id is -1, if it is, set it to 0.
				if (unsll == -1) unsll = 0;

				// Set the data to received obj id.
				this->data = unsll;

				// Print LWOOBJID data.
				// printData();
			}

			/*
				Prints on the console that this LWOOBJID was generated.
			*/
			void printData() {
				// Static List of Category names for easy string build.
				static const std::string CATEGORY_NAMES[4] = { "STATIC","LOCAL","GLOBAL","SPAWNED" };

				// Build the log screen with data, flags, pureID and category name.
				std::stringstream ss; ss << this->data << " [ " << (int)this->getFlags() << " | " << this->getPureID() << " ] -> " << CATEGORY_NAMES[(int)this->getCategory()];
				
				// Log it.
				Logger::log("LWOOBJID", ss.str());
			}

			/*
				Returns this LWOOBJID flags.
			*/
			unsigned char getFlags() {
				// Return the flag.
				return (this->data >> (idSize - 1)) & 0xFF;
			}

			/*
				Return this LWOOBJID pure ID.
			*/
			std::uint64_t getPureID() {
				// Return the pure ID.
				return (this->data & std::uint64_t(0x00FFFFFFFFFFFFFF));
			}

			/*
				Returns this LWOOBJID category.
			*/
			CATEGORY getCategory() {
				// Get flags.
				unsigned char flags = getFlags();

				// Check for Spawned obj id.
				if (flags == (1 << (57 - idSize)))
					return CATEGORY::SPAWNED;

				// Check for Global obj id.
				unsigned char highest5 = (flags >> (flagSize - 5));
				if (highest5 && !(highest5 & (highest5 - 1)))
					return CATEGORY::GLOBAL;

				// Check for Local obj id.
				if ((flags & (std::uint64_t(1) << (57 - idSize))) && (flags & (std::uint64_t(1) << (45 - idSize))))
					return CATEGORY::LOCAL;

				// Check for Static obj id.
				if ((flags >> (flagSize - 6)) == NULL)
					return CATEGORY::STATIC;

				// Something went wrong, we could not find the category, log it and throw exeption.
				Logger::log("LWOOBJID", "Impossible flag category", LogType::UNEXPECTED);
				throw std::runtime_error("Impossible Flag Category.");
			}

			/*
				Makes Player OBJID
			*/
			static LWOOBJID makePlayerObjectID(std::uint64_t base) {
				return 0x1000000000000000 | (base & 0xffffffff);
			}
	};
}

#endif // !_LWOOBJID_HPP__