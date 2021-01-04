#ifndef	__FILETYPES__RAWFILE__RAWHEIGHTMAP_HPP__
#define __FILETYPES__RAWFILE__RAWHEIGHTMAP_HPP__

#include <memory>
#include <cmath>
#include <list>


namespace RAW {
	class RAWChunk;
	class RAWHeightmap {
	public:
		std::uint32_t unknownA;
		std::uint32_t unknownB;
		std::uint32_t unknownC;
		std::uint32_t unknownD;

		std::float_t scaleFactor;

		std::list<std::float_t> map = {};

		void Read(RAW::RAWChunk* chunk, std::uint8_t** fileDataPtr);
	};
};

#endif