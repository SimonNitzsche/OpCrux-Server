#ifndef __DATATYPES__QUATERNION_HPP__
#define __DATATYPES__QUATERNION_HPP__

#include "Vector3.hpp"

namespace DataTypes {

	// Defines some types for better accessbility.
	typedef Vector3 GENERIC_POSITION;
	typedef Vector3 GENERIC_VELOCITY;
	typedef Vector3 GENERIC_VELOCITY_ANGULAR;

	// Defines some function to be used on the Quaternion.
	#define radiansBetween(loc1, loc2) atan2(loc1.z - loc2.z,  loc1.x - loc2.x);
	#define angle_2D(loc1, loc2) atan2(loc1.x * loc2.z - loc1.z * loc2.x, loc1.x * loc2.x + loc1.z * loc2.z)
	#define angle_2D_New(loc1, loc2) atan2(loc1.z - loc2.z, loc1.x - loc2.x) / M_PI

	// Defines more functions to be used on the Quaternion.
	#define RAD2DEG 57.29578049f;
	#define DEG2RAD 0.01745329f;

	// Defines Static Flag Sign_D for future operations.
	static float Sign_D(float i) { return (i >-0.0f) ? 1.0f : -1.0f; }

	/*
		The Rotation Axis of an Object.
	*/
	struct Quaternion {

		// Quaternion use 4 axis: x, y, z and w.
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;

		/*
			Constructs an empty Quaternion.
		*/
		Quaternion() {}

		/*
			Constructs a Quaternion with x, y, z and w axis.
		*/
		Quaternion(float x, float y, float z, float w) {
			// Set x, y, z and w.
			this->x = x; this->y = y; this->z = z; this->w = w;
		}

		/*
			Wheather a Quaternion is null.
		*/
		bool isNull() {
			// Check for all axis if they are 0.
			return (this->x == 0 && this->y == 0 && this->z == 0 && this->w == 0);
		}

		/*
			Multiplies two Quaternions.
		*/
		Quaternion operator * (const Quaternion & q) const {
			// Create an empty quaternion.
			Quaternion qu;

			// Custom multiplication operation for w, x, y and z axis.
			qu.w = w * q.w - x * q.x - y * q.y - z * q.z;
			qu.x = w * q.x + x * q.w + y * q.z - z * q.y;
			qu.y = w * q.y - x * q.z + y * q.w + z * q.x;
			qu.z = w * q.z + x * q.y - y * q.x + z * q.w;

			// Return our new quaternion.
			return qu;
		}

		/*
			Adds two Quaternions.
		*/
		Quaternion operator + (const Quaternion & q) const {
			// Return a new quaternion with the result.
			return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
		}

		/*
			Sets a Quaternion to another Quaternion.
		*/
		Quaternion & operator = (const Quaternion & q) {
			// Set x, y, z and w data.
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;

			// Return this Quaternion pointer.
			return *this;
		}

		/*
			Multiplies x, y, z and w by a new value and sets.
		*/
		Quaternion & operator *= (float v) {
			// Multiply and set x, y, z and w.
			x *= v;
			y *= v;
			z *= v;
			w *= v;

			// Return this Quaternion pointer.
			return *this;
		}

		/*
			Multiplies x, y, z and w by a new value.
		*/
		Quaternion operator * (float v) {
			// Return a new quaternion with multiplied values.
			return Quaternion(x * v, y * v, z * v, w * v);
		}

		/*
			Multiplies a Quaternion by a Vector3.
		*/
		Vector3 operator * (Vector3 vector) {
			// Create an empty Quaternion.
			Quaternion vector3Quat;

			// Set the quaternion's x, y, z to the vector's position.
			vector3Quat.x = vector.x;
			vector3Quat.y = vector.y;
			vector3Quat.z = vector.z;

			// Set the quaternion's w to 0.
			vector3Quat.w = 0.0f;

			// Invert this Quaternion.
			Quaternion inverseQuat = Quaternion::invert(*this);

			// Combine the vector3 quaternion with this inverted quaternion and this quaternion.
			Quaternion combined = inverseQuat * vector3Quat * (*this);

			// Return the combined value.
			return Vector3(combined.x, combined.y, combined.z);

			// This will never execute, I don't know what's this doing here?
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}

		/*
			Wrapper functions to rotate the Quaternion in x, y and z axis.
		*/
		void rotateX(float radians) { Quaternion::rotateX(*this, radians); }
		void rotateY(float radians) { Quaternion::rotateY(*this, radians); }
		void rotateZ(float radians) { Quaternion::rotateZ(*this, radians); }

		/*
			Rotates a Quaternion on x axis.
		*/
		static void rotateX(Quaternion & q, float radians) {
			// Transforms the radians into a Quaternion on X axis.
			Quaternion qu = Quaternion::toQuaternion(Vector3(radians, 0.0f, 0.0f));

			// Slerp the Quaternion with qu.
			Quaternion::slerp(q, qu, 1.0f, 0.05f);
		}

		/*
			Rotates a Quaternion on y axis.
		*/
		static void rotateY(Quaternion & q, float radians) {
			// Transforms the radians into a Quaternion on Y axis.
			Quaternion qu = Quaternion::toQuaternion(Vector3(0.0f, radians, 0.0f));

			// Slerp the Quaternion with qu.
			Quaternion::slerp(q, qu, 1.0f, 0.05f);
		}

		/*
			Rotates a Quaternion on z axis.
		*/
		static void rotateZ(Quaternion & q, float radians) {
			// Transforms the radians into a Quaternion on Z axis.
			Quaternion qu = Quaternion::toQuaternion(Vector3(0.0f, 0.0f, radians));

			// Slerp the Quaternion with qu.
			Quaternion::slerp(q, qu, 1.0f, 0.05f);
		}

		/*
			Transforms a Quaternion into a string.
		*/
		std::string makeString() {
			// Create the string.
			std::string ss = "";

			// Format x, y, z and w axis into the string.
			ss += "{ "; ss += std::to_string(x);
			ss += ", "; ss += std::to_string(y);
			ss += ", "; ss += std::to_string(z);
			ss += ", " + std::to_string(w) + "  }";

			// Return it.
			return ss;
		}

		/*
			Slerps a Quaternion.
			Courtesy of https://svn.code.sf.net/p/irrlicht/code/trunk/include/quaternion.h
		*/
		static float slerp(Quaternion & q1, Quaternion q2, float progress, float accuracyThreshold = 0.05f) {
			float angle = Quaternion::dotProduct(q1, q2);
			float direction = 1.0f;

			if (angle < 0.0f) {
				q1 *= -1.0f;
				angle *= -1.0f;
				direction = -1.0f;
			}

			if (angle <= (1 - accuracyThreshold)) {
				float theta = acosf(angle);
				float invSinTheta = 1 / (sinf(theta));

				float scale = sinf(theta * (1.0f - progress)) * invSinTheta;
				float inverseScale = sinf(theta * progress) * invSinTheta;

				q1 = q1 * scale + q2 * inverseScale;
			} else {
				float scale = 1.0f - progress;
				q1 = q1 * scale + q2 * progress;
			}

			return direction;
		}

		/*
			Invertes a Quaternion.
		*/
		static Quaternion invert(Quaternion qu) {
			// Create an empty Quaternion.
			Quaternion qua;

			// Calculate the inverse dot product.
			float inverseDotProduct = 1.0f / Quaternion::dotProduct(qu, qu);

			// Multiply x, y, z and w by the inverse dot product.
			qua.x *= -inverseDotProduct;
			qua.y *= -inverseDotProduct;
			qua.z *= -inverseDotProduct;
			qua.w *= inverseDotProduct;

			// Return the new Quaternion.
			return qua;
		}

		/*
			Calculates the dot product between two quaternions.
		*/
		static float dotProduct(Quaternion qu1, Quaternion qu2) {
			// Return dot product.
			return qu1.x * qu2.x + qu1.y * qu2.y + qu1.z * qu2.z + qu1.w * qu2.w;
		}

		/*
			Returns the length of a Quaternion.
		*/
		static float length(Quaternion qu) {
			// Return square root of the dot product between itself.
			return sqrt(Quaternion::dotProduct(qu, qu));
		}

		/*
			Normalizes the Quaternion.
		*/
		void normalize() {
			// Get the length of the quaternion.
			float length = Quaternion::length(*this);

			// Divide x, y, z and w by the length.
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		/*
			Creates a Quaternion from an Axis Angle.
		*/
		static Quaternion fromAxisAngle(Vector3 axis, float angle) {
			// Create an empty quaternion.
			Quaternion qu;

			// Calculate half angle sign.
			float halfAngleSign = sin(angle / 2);

			// Set x, y, z as the axis * half angle sign.
			qu.x = axis.x * halfAngleSign;
			qu.y = axis.y * halfAngleSign;
			qu.z = axis.z * halfAngleSign;

			// Cos of the angle / 2 is the value for w.
			qu.w = cos(angle / 2);

			// Finally, normalize the quaternion.
			qu.normalize();

			// Return it.
			return qu;
		}

		/*
			Creates a Quaternion from a Rotation Matrix.
		*/
		static Quaternion fromRotationMatrix(float * matrix) {
			// Calculate x, y, z and w with the float matrix pointer.
			float x = (matrix[0] - matrix[5] - matrix[10] + 1.0f) / 4.0f;
			float y = (-matrix[0] + matrix[5] - matrix[10] + 1.0f) / 4.0f;
			float z = (-matrix[0] - matrix[5] + matrix[10] + 1.0f) / 4.0f;
			float w = (matrix[0] + matrix[5] + matrix[10] + 1.0f) / 4.0f;

			// If by any case x, y, z or w is negative, reset it to 0.
			if (x < 0.0f) x = 0.0f;
			if (y < 0.0f) y = 0.0f;
			if (z < 0.0f) z = 0.0f;
			if (w < 0.0f) w = 0.0f;

			// Set x, y, z and w to they're respective square roots.
			x = sqrt(x);
			y = sqrt(y);
			z = sqrt(z);
			w = sqrt(w);

			// If by any case this is true.
			if (w >= x && w >= y && w >= z) {
				// Multiply set x, y and z by they signs.
				x *= Sign_D(matrix[7] - matrix[9]);
				y *= Sign_D(matrix[8] - matrix[2]);
				z *= Sign_D(matrix[1] - matrix[4]);
				//w *= 1.0f;
			} else if (x >= w && x >= y && x >= z) {
				// Multiply set y, z and w by they signs.
				//x *= 1.0f;
				y *= Sign_D(matrix[1] + matrix[4]);
				z *= Sign_D(matrix[8] + matrix[2]);
				w *= Sign_D(matrix[7] - matrix[9]);
			} else if (y >= w && y >= x && y >= z) {
				// Multiply set x, z and w by they signs.
				x *= Sign_D(matrix[1] + matrix[4]);
				//y *= 1.0f;
				z *= Sign_D(matrix[7] + matrix[9]);
				w *= Sign_D(matrix[8] - matrix[2]);
			} else if (z >= w && z >= x && z >= y) {
				// Multiply set x, y and w by they signs.
				x *= Sign_D(matrix[2] + matrix[8]);
				y *= Sign_D(matrix[7] + matrix[9]);
				//z *= 1.0f;
				w *= Sign_D(matrix[1] - matrix[4]);
			} else {
				// Something went really wrong... write it on the console.
				puts("Quaternion::fromRotationMatrix error!!"); // (As if this should ever happen)

				// Return an empty quaternion.
				return Quaternion();
			}

			// Create a Quaternion with the x, y, z and w values.
			Quaternion qu = Quaternion(x, y, z, w);

			// Normalize it.
			qu.normalize();

			// Return it.
			return qu;
		}

		/*
			Transforms a Vector3 into a Quaternion.
		*/
		static Quaternion toQuaternion(Vector3 euler) {
			// Creates an empty quaternion.
			Quaternion q;

			// Calculate t0, t1, t2, t3, t4, t5 with cos/sin of the vector's axis multiplied by 0.5f.
			float t0 = cos(euler.z * 0.5f);
			float t1 = sin(euler.z * 0.5f);
			float t2 = cos(euler.x * 0.5f);
			float t3 = sin(euler.x * 0.5f);
			float t4 = cos(euler.y * 0.5f);
			float t5 = sin(euler.y * 0.5f);

			// Make absurd calculations to set the w, x, y and z axis on the quaternion.
			q.w = t0 * t2 * t4 + t1 * t3 * t5;
			q.x = t0 * t3 * t4 - t1 * t2 * t5;
			q.y = t0 * t2 * t5 + t1 * t3 * t4;
			q.z = t1 * t2 * t4 - t0 * t3 * t5;

			// Return the Quaternion.
			return q;
		}

		/*
			Transforms a Quaternion into a Vector3.
		*/
		static Vector3 toEuler(const Quaternion & q) {
			// Calculate ysqr with a multiplication of the quaternion's y by itself.
			float ysqr = q.y * q.y;

			// Calculates roll (x-axis rotation)
			float t0 = +2.0f * (q.w * q.x + q.y * q.z);
			float t1 = +1.0f - 2.0f * (q.x * q.x + ysqr);
			float roll = atan2(t0, t1);

			// Calculates pitch (y-axis rotation)
			float t2 = +2.0f * (q.w * q.y - q.z * q.x);
			t2 = t2 > 1.0f ? 1.0f : t2;
			t2 = t2 < -1.0f ? -1.0f : t2;
			float pitch = asin(t2);

			// Calculates yaw (z-axis rotation)
			float t3 = +2.0f * (q.w * q.z + q.x * q.y);
			float t4 = +1.0f - 2.0f * (ysqr + q.z * q.z);
			float yaw = atan2(t3, t4);

			// Returns a new Vector 3 with roll, pitch and yaw.
			return Vector3(roll, pitch, yaw);
		}

		/*
			Return this Quaternions yaw value.
		*/
		float yaw() {
			return atan2(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
		}

		/*
			Returns a rotation value between two vector3 points.
			Code derived from: https://stackoverflow.com/questions/14337441; https://pastebin.com/9JBMWizm.
		*/
		static Quaternion betweenPoints(Vector3 origin, Vector3 destination) {
			// Get the vector3 between the two points.
			Vector3 vectorBetween = destination - origin;
			
			// Normalize the vector.
			vectorBetween.normalize(true);

			// Gets the cross between the vector and the y axis.
			Vector3 axisX = Vector3::Cross(vectorBetween, Vector3(0, -1, 0)); // Controls which axis's rotate

			// Normalize it.
			axisX.normalize();

			// Gets the cross between the vector and the axisX.
			Vector3 axisY = Vector3::Cross(vectorBetween, axisX);

			// Normalize it.
			axisY.normalize();

			// Creates a rotation matrix for this new Quaternion.
			float rotationMatrix[16] = {
				axisX.x, axisX.y, axisX.z,
				0.0f,
				axisY.x, axisY.y, axisY.z,
				0.0f,
				vectorBetween.x, vectorBetween.y, vectorBetween.z,
				0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f
			};

			// Create a quaternion from the rotation matrix and return it.
			return Quaternion::fromRotationMatrix(rotationMatrix);
		}
	};
}

#endif // !__DATATYPES__QUATERNION_HPP__