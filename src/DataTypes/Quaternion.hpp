#ifndef __DATATYPES__QUATERNION_HPP__
#define __DATATYPES__QUATERNION_HPP__

#include "Vector3.hpp"
typedef Vector3 GENERIC_POSITION;
typedef Vector3 GENERIC_VELOCITY;
typedef Vector3 GENERIC_VELOCITY_ANGULAR;

#define radiansBetween(loc1, loc2) atan2(loc1.z - loc2.z,  loc1.x - loc2.x);
#define angle_2D(loc1, loc2) atan2(loc1.x * loc2.z - loc1.z * loc2.x, loc1.x * loc2.x + loc1.z * loc2.z)
#define angle_2D_New(loc1, loc2) atan2(loc1.z - loc2.z, loc1.x - loc2.x) / M_PI

#define RAD2DEG 57.29578049f;
#define DEG2RAD 0.01745329f;

static float Sign_D(float i) { return (i >-0.0f) ? 1.0f : -1.0f; }

struct Quaternion {
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
	Quaternion() {}
	Quaternion(float x, float y, float z, float w) {
		this->x = x; this->y = y; this->z = z; this->w = w;
	}

	bool isNull() {
		return (this->x == 0 && this->y == 0 && this->z == 0 && this->w == 0);
	}

	Quaternion operator * (const Quaternion & q) const {
		Quaternion qu;
		qu.w = w * q.w - x * q.x - y * q.y - z * q.z;
		qu.x = w * q.x + x * q.w + y * q.z - z * q.y;
		qu.y = w * q.y - x * q.z + y * q.w + z * q.x;
		qu.z = w * q.z + x * q.y - y * q.x + z * q.w;
		return qu;
	}

	Quaternion operator + (const Quaternion & q) const {
		return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	Quaternion & operator = (const Quaternion & q) {
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
		return *this;
	}

	Quaternion & operator *= (float v) {
		x *= v;
		y *= v;
		z *= v;
		w *= v;
		return *this;
	}

	Quaternion operator * (float v) {
		return Quaternion(x * v, y * v, z * v, w * v);
	}

	Vector3 operator * (Vector3 vector) {
		Quaternion vector3Quat;
		vector3Quat.x = vector.x;
		vector3Quat.y = vector.y;
		vector3Quat.z = vector.z;
		vector3Quat.w = 0.0f;

		Quaternion inverseQuat = Quaternion::invert(*this);

		Quaternion combined = inverseQuat * vector3Quat * (*this);

		return Vector3(combined.x, combined.y, combined.z);
		return Vector3(x * vector.x, y * vector.y, z * vector.z);
	}

	void rotateX(float radians) { Quaternion::rotateX(*this, radians); }
	void rotateY(float radians) { Quaternion::rotateY(*this, radians); }
	void rotateZ(float radians) { Quaternion::rotateZ(*this, radians); }

	static void rotateX(Quaternion & q, float radians) {
		Quaternion qu = Quaternion::toQuaternion(Vector3(radians, 0.0f, 0.0f));
		Quaternion::slerp(q, qu, 1.0f, 0.05f);
	}

	static void rotateY(Quaternion & q, float radians) {
		Quaternion qu = Quaternion::toQuaternion(Vector3(0.0f, radians, 0.0f));
		Quaternion::slerp(q, qu, 1.0f, 0.05f);
	}

	static void rotateZ(Quaternion & q, float radians) {
		Quaternion qu = Quaternion::toQuaternion(Vector3(0.0f, 0.0f, radians));
		Quaternion::slerp(q, qu, 1.0f, 0.05f);
	}

	std::string makeString() {
		std::string ss = "";
		ss += "{ "; ss += std::to_string(x);
		ss += ", "; ss += std::to_string(y);
		ss += ", "; ss += std::to_string(z);
		ss += ", " + std::to_string(w) + "  }";
		return ss;
	}

	// Courtesy of https://svn.code.sf.net/p/irrlicht/code/trunk/include/quaternion.h
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
		}
		else {
			float scale = 1.0f - progress;
			q1 = q1 * scale + q2 * progress;
		}
		return direction;
	}

	static Quaternion invert(Quaternion qu) {
		Quaternion qua;
		float inverseDotProduct = 1.0f / Quaternion::dotProduct(qu, qu);
		qua.x *= -inverseDotProduct;
		qua.y *= -inverseDotProduct;
		qua.z *= -inverseDotProduct;
		qua.w *= inverseDotProduct;

		return qua;
	}

	static float dotProduct(Quaternion qu1, Quaternion qu2) {
		return qu1.x * qu2.x + qu1.y * qu2.y + qu1.z * qu2.z + qu1.w * qu2.w;
	}

	static float length(Quaternion qu) {
		return sqrt(Quaternion::dotProduct(qu, qu));
	}

	void normalize() {
		float length = Quaternion::length(*this);
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	static Quaternion fromAxisAngle(Vector3 axis, float angle) {
		Quaternion qu;

		float halfAngleSign = sin(angle / 2);

		qu.x = axis.x * halfAngleSign;
		qu.y = axis.y * halfAngleSign;
		qu.z = axis.z * halfAngleSign;
		qu.w = cos(angle / 2);

		qu.normalize();

		return qu;
	}

	static Quaternion fromRotationMatrix(float * matrix) {
		float x = (matrix[0] - matrix[5] - matrix[10] + 1.0f) / 4.0f;
		float y = (-matrix[0] + matrix[5] - matrix[10] + 1.0f) / 4.0f;
		float z = (-matrix[0] - matrix[5] + matrix[10] + 1.0f) / 4.0f;
		float w = (matrix[0] + matrix[5] + matrix[10] + 1.0f) / 4.0f;

		if (x < 0.0f) x = 0.0f;
		if (y < 0.0f) y = 0.0f;
		if (z < 0.0f) z = 0.0f;
		if (w < 0.0f) w = 0.0f;

		x = sqrt(x);
		y = sqrt(y);
		z = sqrt(z);
		w = sqrt(w);

		if (w >= x && w >= y && w >= z) {
			x *= Sign_D(matrix[7] - matrix[9]);
			y *= Sign_D(matrix[8] - matrix[2]);
			z *= Sign_D(matrix[1] - matrix[4]);
			//w *= 1.0f;
		}

		else if (x >= w && x >= y && x >= z) {
			//x *= 1.0f;
			y *= Sign_D(matrix[1] + matrix[4]);
			z *= Sign_D(matrix[8] + matrix[2]);
			w *= Sign_D(matrix[7] - matrix[9]);
		}

		else if (y >= w && y >= x && y >= z) {
			x *= Sign_D(matrix[1] + matrix[4]);
			//y *= 1.0f;
			z *= Sign_D(matrix[7] + matrix[9]);
			w *= Sign_D(matrix[8] - matrix[2]);
		}

		else if (z >= w && z >= x && z >= y) {
			x *= Sign_D(matrix[2] + matrix[8]);
			y *= Sign_D(matrix[7] + matrix[9]);
			//z *= 1.0f;
			w *= Sign_D(matrix[1] - matrix[4]);
		}

		else {
			puts("Quaternion::fromRotationMatrix error!!"); // (As if this should ever happen)
			return Quaternion();
		}

		Quaternion qu = Quaternion(x, y, z, w);
		qu.normalize();
		return qu;

	}

	static Quaternion toQuaternion(Vector3 euler) {
		Quaternion q;
		float t0 = cos(euler.z * 0.5f);
		float t1 = sin(euler.z * 0.5f);
		float t2 = cos(euler.x * 0.5f);
		float t3 = sin(euler.x * 0.5f);
		float t4 = cos(euler.y * 0.5f);
		float t5 = sin(euler.y * 0.5f);

		q.w = t0 * t2 * t4 + t1 * t3 * t5;
		q.x = t0 * t3 * t4 - t1 * t2 * t5;
		q.y = t0 * t2 * t5 + t1 * t3 * t4;
		q.z = t1 * t2 * t4 - t0 * t3 * t5;
		return q;
	}

	static Vector3 toEuler(const Quaternion & q) {
		float ysqr = q.y * q.y;

		// roll (x-axis rotation)
		float t0 = +2.0f * (q.w * q.x + q.y * q.z);
		float t1 = +1.0f - 2.0f * (q.x * q.x + ysqr);
		float roll = atan2(t0, t1);

		// pitch (y-axis rotation)
		float t2 = +2.0f * (q.w * q.y - q.z * q.x);
		t2 = t2 > 1.0f ? 1.0f : t2;
		t2 = t2 < -1.0f ? -1.0f : t2;
		float pitch = asin(t2);

		// yaw (z-axis rotation)
		float t3 = +2.0f * (q.w * q.z + q.x * q.y);
		float t4 = +1.0f - 2.0f * (ysqr + q.z * q.z);
		float yaw = atan2(t3, t4);

		return Vector3(roll, pitch, yaw);
	}

	float yaw() {
		return atan2(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
	}

	// Code derived from|
	// https://stackoverflow.com/questions/14337441
	// and
	// https://pastebin.com/9JBMWizm
	static Quaternion betweenPoints(Vector3 origin, Vector3 destination) {
		Vector3 vectorBetween = destination - origin;
		vectorBetween.normalize(true);

		Vector3 axisX = Vector3::Cross(vectorBetween, Vector3(0, -1, 0)); // Controls which axis's rotate
		axisX.normalize();

		Vector3 axisY = Vector3::Cross(vectorBetween, axisX);
		axisY.normalize();

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

		return Quaternion::fromRotationMatrix(rotationMatrix);
	}
};

#endif // !__DATATYPES__QUATERNION_HPP__