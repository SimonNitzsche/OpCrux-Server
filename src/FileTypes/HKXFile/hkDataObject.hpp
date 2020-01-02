#ifndef __FILETYPES__HKXFILE__HKDATAOBJECT_HPP__
#define __FILETYPES__HKXFILE__HKDATAOBJECT_HPP__

namespace HKX {
	class hkDataObject {
		enum class Type {
			/// Not reflected.
			TYPE_VOID = 0,

			/// Byte, signed or unsigned 8-bit integer, used only in arrays and tuples
			TYPE_BYTE,
			/// Signed or unsigned 8/16/32/64-bit integer. 
			TYPE_INT,
			/// 32-bit float.
			TYPE_REAL,
			/// Fixed array of 4 TYPE_REAL (e.g. hkVector, hkQuaternion).
			TYPE_VEC_4,
			/// Fixed array of 8 TYPE_REAL.
			TYPE_VEC_8,
			/// Fixed array of 12 TYPE_REAL (e.g. hkMatrix3, hkQsTransform, hkRotation).
			TYPE_VEC_12,
			/// Fixed array of 16 TYPE_REAL (e.g. hkTransform, hkMatrix4).
			TYPE_VEC_16,
			/// hkDataObject.
			TYPE_OBJECT,
			/// hkDataObject (embedded struct data).
			TYPE_STRUCT,
			/// C-style string.
			TYPE_CSTRING,
			/// The number of basic hkDataObject types.
			TYPE_NUM_BASIC_TYPES,
			/// Mask for the basic hkDataObject types.
			TYPE_MASK_BASIC_TYPES = 0xf,

			/// Bit indicating an array of the basic type data.
			TYPE_ARRAY = 0x10, // per object size array
				/// Array of TYPE_BYTE.
				TYPE_ARRAY_BYTE = TYPE_ARRAY | TYPE_BYTE,
				/// Array of TYPE_INT.
				TYPE_ARRAY_INT = TYPE_ARRAY | TYPE_INT,
				/// Array of TYPE_REAL.
				TYPE_ARRAY_REAL = TYPE_ARRAY | TYPE_REAL,
				/// Array of TYPE_VEC_4.
				TYPE_ARRAY_VEC_4 = TYPE_ARRAY | TYPE_VEC_4,
				/// Array of TYPE_VEC_8.
				TYPE_ARRAY_VEC_8 = TYPE_ARRAY | TYPE_VEC_8,
				/// Array of TYPE_VEC_12.
				TYPE_ARRAY_VEC_12 = TYPE_ARRAY | TYPE_VEC_12,
				/// Array of TYPE_VEC_16.
				TYPE_ARRAY_VEC_16 = TYPE_ARRAY | TYPE_VEC_16,
				/// Array of TYPE_OBJECT.
				TYPE_ARRAY_OBJECT = TYPE_ARRAY | TYPE_OBJECT,
				/// Array of TYPE_STRUCT.
				TYPE_ARRAY_STRUCT = TYPE_ARRAY | TYPE_STRUCT,
				/// Array of TYPE_CSTRING.
				TYPE_ARRAY_CSTRING = TYPE_ARRAY | TYPE_CSTRING,

				/// Bit indicating a tuple of the basic type data.
				TYPE_TUPLE = 0x20, // fixed size array, size is per class
					/// Tuple of TYPE_BYTE.
					TYPE_TUPLE_BYTE = TYPE_TUPLE | TYPE_BYTE,
					/// Tuple of TYPE_INT.
					TYPE_TUPLE_INT = TYPE_TUPLE | TYPE_INT,
					/// Tuple of TYPE_REAL.
					TYPE_TUPLE_REAL = TYPE_TUPLE | TYPE_REAL,
					/// Tuple of TYPE_VEC_4.
					TYPE_TUPLE_VEC_4 = TYPE_TUPLE | TYPE_VEC_4,
					/// Tuple of TYPE_VEC_8.
					TYPE_TUPLE_VEC_8 = TYPE_TUPLE | TYPE_VEC_8,
					/// Tuple of TYPE_VEC_12.
					TYPE_TUPLE_VEC_12 = TYPE_TUPLE | TYPE_VEC_12,
					/// Tuple of TYPE_VEC_16.
					TYPE_TUPLE_VEC_16 = TYPE_TUPLE | TYPE_VEC_16,
					/// Tuple of TYPE_OBJECT.
					TYPE_TUPLE_OBJECT = TYPE_TUPLE | TYPE_OBJECT,
					/// Tuple of TYPE_STRUCT.
					TYPE_TUPLE_STRUCT = TYPE_TUPLE | TYPE_STRUCT,
					/// Tuple of TYPE_CSTRING.
					TYPE_TUPLE_CSTRING = TYPE_TUPLE | TYPE_CSTRING
		};
	};
}

#endif