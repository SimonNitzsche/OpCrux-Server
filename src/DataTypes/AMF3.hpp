#pragma once
#ifndef _AMF3_HPP
#define _AMF3_HPP

#include <string>
#include <exception>
#include <RakNet/BitStream.h>

namespace DataTypes {

	/*
		All AMF3 Markers.
	*/
	enum class AMF3_TYPE_MARKER : unsigned char {
		undefined_marker = 0,
		null_marker,
		false_marker,
		true_marker,
		integer_marker,
		double_marker,
		string_marker,
		xml_doc_marker,
		date_marker,
		array_marker,
		object_marker,
		xml_marker,
		byte_array_marker,
		vector_int_marker,
		vector_uint_marker,
		vector_double_marker,
		vector_object_marker,
		dictionary_marker
	};

	/*
		A specific data format U29.
	*/
	class U29 {
		private:
			unsigned data : 29;
		public:

			/*
				Constructs an empty U29.
			*/
			U29() {};

			/*
				Constructs a U29 with an integer.
			*/
			U29(int input) { data = (input >> 3); }

			/*
				Constructs a U29 with a BitStream.
			*/
			U29(RakNet::BitStream * input) {
				data = 0;
				for (int i = 0; i < 4; ++i) {
					unsigned char buffer;
					input->Read(buffer);
					data += ((buffer & 0x7F) << (8 * i));

					if (buffer & 0x80) { // first bit active
						continue;
					}

					break;
				}
			}

			/*
				Writes a U29 to a bit stream.
			*/
			void write(RakNet::BitStream * output) {
				std::uint32_t matcher = 0x7F;
				for (int i = 0; i < 4; i++) {
					bool needNewByteAfter = data & (matcher << (i + 1));
					char buffer = (data & (matcher << i)) + (needNewByteAfter << 7);
					output->Write(buffer);
					if (!needNewByteAfter) return;
				}
			}

			/*
				Get the data as unsigned long.
			*/
			operator unsigned long() const {
				return data;
			}
	};

	/*
		Defines a AMF3 Type.
	*/
	class AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { throw new std::runtime_error("Can't execute on interface."); }
	};

	/*
		The AMF3 Undefined type.
	*/
	class amf3_undefined : public AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { return AMF3_TYPE_MARKER::undefined_marker; }
			amf3_undefined() {};
	};

	/*
		The AMF3 null type.
	*/
	class amf3_null : public AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { return AMF3_TYPE_MARKER::null_marker; }
	};

	/*
		The AMF3 bool type.
	*/
	class amf3_boolean : public AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { return static_cast<AMF3_TYPE_MARKER>(static_cast<unsigned char>(AMF3_TYPE_MARKER::false_marker) + data); }
		private:
			bool data;
		public:
			amf3_boolean(bool input) : data(input) {}
	};

	/*
		The AMF3 String type.
	*/
	class amf3_string : public AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { return AMF3_TYPE_MARKER::string_marker; }
		private:
			std::string data;
		public:
			amf3_string(std::string input) : data(input) {}
			amf3_string(RakNet::BitStream * input) {

			}
	};

	/*
		The AMF3 array type.
	*/
	class amf3_array : public AMF3_TYPE {
		public:
			AMF3_TYPE_MARKER get_type_marker() { return AMF3_TYPE_MARKER::array_marker; }
		private:
		public:
			amf3_array(RakNet::BitStream * input) {

			}
	};

	/*
		The AMF3 Class.
	*/
	class AMF3 {
		public:
			static AMF3_TYPE Read(RakNet::BitStream * bs) {
				AMF3_TYPE_MARKER marker;
				bs->Read(marker);
				switch (marker) {
				case AMF3_TYPE_MARKER::undefined_marker:
					return amf3_undefined();

				case AMF3_TYPE_MARKER::null_marker:
					return amf3_null();

				case AMF3_TYPE_MARKER::false_marker:
					return amf3_boolean(false);

				case AMF3_TYPE_MARKER::true_marker:
					return amf3_boolean(true);

				case AMF3_TYPE_MARKER::string_marker:
					return amf3_string(bs);

				case AMF3_TYPE_MARKER::array_marker:
					return amf3_array(bs);

				default:
					throw new std::runtime_error("Unknown marker.");
				}
			}
	};
}

#endif