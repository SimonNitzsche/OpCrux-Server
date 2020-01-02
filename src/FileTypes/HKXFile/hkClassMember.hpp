#ifndef __FILETYPES__HKXFILE__HKCLASSMEMBER_HPP__
#define __FILETYPES__HKXFILE__HKCLASSMEMBER_HPP__

namespace HKX {
	class hkClassMember {
		enum class Type {
			/// No type
			TYPE_VOID = 0,
			/// hkBool,  boolean type
			TYPE_BOOL,
			/// hkChar, signed char type
			TYPE_CHAR,
			/// hkInt8, 8 bit signed integer type
			TYPE_INT8,
			/// hkUint8, 8 bit unsigned integer type
			TYPE_UINT8,
			/// hkInt16, 16 bit signed integer type
			TYPE_INT16,
			/// hkUint16, 16 bit unsigned integer type
			TYPE_UINT16,
			/// hkInt32, 32 bit signed integer type
			TYPE_INT32,
			/// hkUint32, 32 bit unsigned integer type
			TYPE_UINT32,
			/// hkInt64, 64 bit signed integer type
			TYPE_INT64,
			/// hkUint64, 64 bit unsigned integer type
			TYPE_UINT64,
			/// hkReal, float type
			TYPE_REAL,
			/// hkVector4 type
			TYPE_VECTOR4,
			/// hkQuaternion type
			TYPE_QUATERNION,
			/// hkMatrix3 type
			TYPE_MATRIX3,
			/// hkRotation type
			TYPE_ROTATION,
			/// hkQsTransform type
			TYPE_QSTRANSFORM,
			/// hkMatrix4 type
			TYPE_MATRIX4,
			/// hkTransform type
			TYPE_TRANSFORM,
			/// Serialize as zero - deprecated.
			TYPE_ZERO,
			/// Generic pointer, see member flags for more info
			TYPE_POINTER,
			/// Function pointer
			TYPE_FUNCTIONPOINTER,
			/// hkArray<T>, array of items of type T
			TYPE_ARRAY,
			/// hkInplaceArray<T,N> or hkInplaceArrayAligned16<T,N>, array of N items of type T
			TYPE_INPLACEARRAY,
			/// hkEnum<ENUM,STORAGE> - enumerated values
			TYPE_ENUM,
			/// Object
			TYPE_STRUCT,
			/// Simple array (ptr(typed) and size only)
			TYPE_SIMPLEARRAY,
			/// Simple array of homogeneous types, so is a class id followed by a void* ptr and size
			TYPE_HOMOGENEOUSARRAY,
			/// hkVariant (void* and hkClass*) type
			TYPE_VARIANT,
			/// char*, null terminated string
			TYPE_CSTRING,
			/// hkUlong, unsigned long, defined to always be the same size as a pointer
			TYPE_ULONG,
			/// hkFlags<ENUM,STORAGE> - 8,16,32 bits of named values.
			TYPE_FLAGS,
			/// hkHalf, 16-bit float value
			TYPE_HALF,
			/// hkStringPtr, c-string
			TYPE_STRINGPTR,
			TYPE_MAX
		};
	};
}

#endif