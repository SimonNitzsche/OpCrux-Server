#ifndef _VECTOR3_HPP__
#define _VECTOR3_HPP__

#include <limits>
#include <cmath>
#include <string>
#include <stdexcept>
#include "Common/CrossPlatform.hpp"
#include "RakNet/BitStream.h"
#include <reactphysics3d/reactphysics3d.h>

#ifndef _MATH_DEFINES_DEFINED
#define _MATH_DEFINES_DEFINED
// Definitions of useful mathematical constants
//
// Define _USE_MATH_DEFINES before including <math.h> to expose these macro
// definitions for common math constants.  These are placed under an #ifdef
// since these commonly-defined names are not part of the C or C++ standards
#define M_E        2.71828182845904523536   // e
#define M_LOG2E    1.44269504088896340736   // log2(e)
#define M_LOG10E   0.434294481903251827651  // log10(e)
#define M_LN2      0.693147180559945309417  // ln(2)
#define M_LN10     2.30258509299404568402   // ln(10)
#define M_PI       3.14159265358979323846   // pi
#define M_PI_2     1.57079632679489661923   // pi/2
#define M_PI_4     0.785398163397448309616  // pi/4
#define M_1_PI     0.318309886183790671538  // 1/pi
#define M_2_PI     0.636619772367581343076  // 2/pi
#define M_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
#define M_SQRT2    1.41421356237309504880   // sqrt(2)
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif

namespace DataTypes {

	/*
		The Position Axis of an Object.
	*/
	class Vector3 {
		public:
			// Axis, x, y and z.
#pragma pack(push, 1)
			float x = 0.f;
			float y = 0.f;
			float z=0.f;
#pragma pack(pop)

			/*
				Returns the sqrMagnitude of this Vector.
			*/
			float sqrMagnitude() const { return powf(x, 2) + powf(y, 2) + powf(z, 2); }

			/*
				Returns the magnitude of this Vector.
			*/
			float magnitude() const { return sqrtf(this->sqrMagnitude()); }

			/*
				Returns this vector, normalized.
			*/
			Vector3 normalized() const { return *this / this->magnitude(); }

			// Static vector properties.
			static Vector3 up() { return Vector3(0, 1, 0); }
			static Vector3 down() { return Vector3(0, -1, 0); }
			static Vector3 forward() { return Vector3(0, 0, 1); }
			static Vector3 back() { return Vector3(0, 0, -1); }
			static Vector3 left() { return Vector3(-1, 0, 0); }
			static Vector3 right() { return Vector3(1, 0, 0); }
			static Vector3 positiveInfinity() { return Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()); }
			static Vector3 negativeInfinity() { return Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()); }
			static Vector3 one() { return Vector3(1, 1, 1); }
			static Vector3 zero() { return  Vector3(0, 0, 0); }

			/*
				Constructs an empty vector.
			*/
			Vector3() : x(0), y(0), z(0) {}

			/*
				Constructs a vector with another one as base.
			*/
			Vector3(const Vector3 &base) : x(base.x), y(base.y), z(base.z) {}

			/*
				Constructs a vector with x, y and z values.
			*/
			Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

			/*
				Constructs a vector with a single value.
			*/
			Vector3(float a) : x(a), y(a), z(a) {}

			/*
				Constructs a vector by reading it from a bit stream.
			*/
			Vector3(RakNet::BitStream* bs) { bs->Read(*this); }

			/*
				Weather two vectors are equal to one another.
			*/
			bool equals(Vector3 other) const { return ((this->x == other.x) && (this->y == other.y) && (this->z == other.z)); }

			/*
				Weather a vector has the same value for it's 3 axis.
			*/
			bool equals(float other) const { return ((this->x == other) && (this->y == other) && (this->z == other)); }

			/*
				Sets the x, y and z values of this vector.
			*/
			void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
			
			/*
				Formats this Vector3 into a string representation.
			*/
			std::string toString() const { return "[ " + std::to_string(this->x) + " , " + std::to_string(this->y) + " , " + std::to_string(this->z) + " ]"; }
			
			/*
				Normalizes this vector.
			*/
			float normalize(bool inverse = false) { float hypo = Dot(*this, *this); if (inverse) hypo = -hypo; this->x /= hypo; this->y /= hypo; this->z /= hypo; return hypo; }

			/*
				Return the dot product between two vectors.
			*/
			static float Dot(Vector3 lhs, Vector3 rhs) { return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z; }

			/*
				Return the cross between two vectors.
			*/
			static Vector3 Cross(Vector3 lhs, Vector3 rhs) { return Vector3(lhs.y*rhs.z - lhs.z*rhs.y, lhs.z*rhs.x - lhs.x*rhs.z, lhs.x*rhs.y - lhs.y*rhs.x); }
			
			/*
				Returns the angle between two vectors.
			*/
			static float Angle(Vector3 from, Vector3 to) { return radiantodegree(acosf(Dot(from, to) / (from.magnitude()*to.magnitude()))); }
			
			/*
				Returns the clamped magnitude of a vector and max length.
			*/
			static Vector3 ClampMagnitude(Vector3 vector, float maxLength) { return vector.normalized()*maxLength; }

			/*
				Returns the distance between two vectors.
			*/
			static float Distance(Vector3 a, Vector3 b) { return  sqrtf(Vector3::Dot((a - b),(a-b))); }

			/*
				Lerps a vector to another by a rate. (clamped)
			*/
			static Vector3 Lerp(Vector3 a, Vector3 b, float t) { clamp(t); return LerpUnclamped(a, b, t); }

			/*
				Lerps a vector to another by a rate. (unclamped)
			*/
			static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t) { return a + (b - a)*t; }

			/*
				Returns the max between two vectors.
			*/
			static Vector3 Max(Vector3 lhs, Vector3 rhs) { return Vector3((lhs.x > rhs.x ? lhs.x : rhs.x), (lhs.y > rhs.y ? lhs.y : rhs.y), (lhs.z > rhs.z ? lhs.z : rhs.z)); }
			
			/*
				Returns the min between two vectors.
			*/
			static Vector3 Min(Vector3 lhs, Vector3 rhs) { return Vector3((lhs.x < rhs.x ? lhs.x : rhs.x), (lhs.y < rhs.y ? lhs.y : rhs.y), (lhs.z < rhs.z ? lhs.z : rhs.z)); }
			
			/*
				Moves a vector towards another.
			*/
			static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {
				Vector3 delta = target - current;
				if (delta.magnitude() < maxDistanceDelta) { return target; }
				else { return current + delta.normalized()*maxDistanceDelta; }
			}
			
			/*
				Normalizes a vector.
			*/
			static Vector3 Normalize(Vector3 & value) { value = value.normalized(); return value; } // Why return a Vector3?

			/*
				Rotates a vector towards another.
			*/
			static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta) {
				float angle = degreetoradian(Angle(current, target));
				if (maxRadiansDelta > angle) { maxRadiansDelta = angle; }
				else if (maxRadiansDelta < angle - M_PI) { maxRadiansDelta = (float)(angle - M_PI); }
				Vector3 retVector = rotate(current, Cross(current, target), maxRadiansDelta);
				float magnitudeDelta = (target.magnitude() - current.magnitude())*maxRadiansDelta / angle;
				if (magnitudeDelta > maxMagnitudeDelta) { magnitudeDelta = maxMagnitudeDelta; }
				float magnitude = retVector.magnitude();
				return retVector * ((magnitude + magnitudeDelta) / magnitude);
			}

			/*
				Scales a vector with another.
			*/
			static Vector3 Scale(Vector3 a, Vector3 b) { return Vector3(a.x*b.x, a.y*b.y, a.z*b.z); }

			/*
				Getst he signed angle between two vectors on an axis.
			*/
			static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis) {
				Normalize(from); Normalize(to); Normalize(axis);
				float angle = atan2(Dot(axis, Cross(from, to)), Dot(from, to));
				return angle * 180 / (float)M_PI;
			}

			/*
				Slerps a vector.
			*/
			static Vector3 Slerp(Vector3 a, Vector3 b, float t) {
				clamp(t);
				Vector3 cross = Cross(a, b);
				return rotate(a, cross, degreetoradian(Angle(a, b))*t).normalized()*(b.magnitude()*t + a.magnitude()*(1 - t));
			}

			/*
				Adds two vectors.
			*/
			Vector3 operator + (Vector3 other) const { return Vector3(this->x + other.x, this->y + other.y, this->z + other.z); }

			/*
				Adds a float to each vector axis.
			*/
			Vector3 operator + (float other) const { return Vector3(this->x + other, this->y + other, this->z + other); }

			/*
				Subtracts two vectors.
			*/
			Vector3 operator - (Vector3 other) const { return Vector3(this->x - other.x, this->y - other.y, this->z - other.z); }

			/*
				Subtract a float from each vector axis.
			*/
			Vector3 operator - (float other) const { return Vector3(this->x - other, this->y - other, this->z - other); }

			/*
				Multiplies two vectors.
			*/
			Vector3 operator * (Vector3 other) const { return Vector3(this->x*other.x, this->y*other.y, this->z*other.z); }

			/*
				Multiples a float with each vector axis.
			*/
			Vector3 operator * (float other) const { return Vector3(this->x*other, this->y*other, this->z*other); }

			/*
				Divide two vectors.
			*/
			Vector3 operator / (Vector3 other) const { return Vector3(this->x / other.x, this->y / other.y, this->z / other.z); }

			/*
				Divides each vector axis by a float.
			*/
			Vector3 operator / (float other) const { return Vector3(this->x / other, this->y / other, this->z / other); }

			/*
				Adds and sets a vector with another vector.
			*/
			void operator += (Vector3 other) { *this = *this + other; }

			/*
				Adds and sets a vector with a float.
			*/
			void operator += (float other) { *this = *this + other; }

			/*
				Subtracts and sets a vector with another vector.
			*/
			void operator -= (Vector3 other) { *this = *this - other; }

			/*
				Subtracts and sets a vector with a float.
			*/
			void operator -= (float other) { *this = *this - other; }

			/*
				Multiplies and sets a vector with another vector.
			*/
			void operator *= (Vector3 other) { *this = *this*other; }

			/*
				Multiplies and sets a vector with a float.
			*/
			void operator *= (float other) { *this = *this*other; }

			/*
				Divides and sets a vector with another vector.
			*/
			void operator /= (Vector3 other) { *this = *this / other; }

			/*
				Divides and sets a vector with a float.
			*/
			void operator /= (float other) { *this = *this / other; }

			/*
				Returns weather two vectors are equal to one another.
			*/
			bool operator == (Vector3 other) const {
				return (((this->x > other.x - 0.00001) && (this->x < other.x + 0.00001))
					&& ((this->y > other.y - 0.00001) && (this->y < other.y + 0.00001))
					&& ((this->z > other.z - 0.00001) && (this->z < other.z + 0.00001)));
			}

			/*
				Returns weather all axis values are equal to a number.
			*/
			bool operator == (float other) const {
				return (((this->x > other - 0.00001) && (this->x < other + 0.00001))
					&& ((this->y > other - 0.00001) && (this->y < other + 0.00001))
					&& ((this->z > other - 0.00001) && (this->z < other + 0.00001)));
			}

			/*
				Returns weather two vectors are not equal to one another.
			*/
			bool operator != (Vector3 other) const { return !(*this == other); }

			/*
				Returns weather all axis values are not equal to a number.
			*/
			bool operator != (float other) const { return !(*this == other); }

			/*
				For Accessbility, array operator to access x, y and z.
			*/
			float& operator [] (int index) {
				switch (index) {
					case 0:	return this->x;
					case 1:	return this->y;
					case 2:	return this->z;
					default: throw new std::runtime_error("Index out of bounds");
				}
			}

			/*
				Returns btVector3
			*/
			const reactphysics3d::Vector3 getReact() const {
				return reactphysics3d::Vector3(x, y, z);
			}

			/*
				returns pointer to float array [x,y,z]
			*/
			std::float_t* getFArr() {
				return reinterpret_cast<std::float_t*>(this);
			}

		private:

			/*
				Rotates a vector in an axis with an angle.
			*/
			static Vector3 rotate(Vector3 vec, Vector3 axis, float angle) {
				Normalize(axis);
				return vec * cos(angle) + Cross(axis, vec)*sin(angle) + axis * Dot(axis, vec)*(1 - cos(angle));
			}

			/*
				Clamps a float.
			*/
			static void clamp(float &t) {
				if (t < 0) { t = 0; }
				if (t > 1) { t = 1; }
			}

			/*
				Converts Radians to Degrees.
			*/
			static float radiantodegree(float rad) { return rad * 180 / (float)M_PI; }

			/*
				Converts Degress to Radians.
			*/
			static float degreetoradian(float deg) { return deg * (float)M_PI / 180; }
	};
}

#endif // !_VECTOR3_HPP__
