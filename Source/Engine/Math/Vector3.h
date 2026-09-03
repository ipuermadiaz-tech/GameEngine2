#pragma once
#include <cmath>
#include <cassert>

namespace nu
{
	struct Vector3
	{
		float x{ 0.0f }, y{ 0.0f }, z{ 0.0f };

		Vector3() = default;
		Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
		Vector3(float v) : x{ v }, y{ v }, z{ v } {}

		float operator [] (unsigned int i) const {
			assert(i < 3); return (&x)[i];
		}
		float& operator [] (unsigned int i) {
			assert(i < 3); return (&x)[i];
		}

		bool operator == (const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }
		bool operator != (const Vector3& v) const { return !(*this == v); }

		Vector3 Add(const Vector3& v) const {
			return Vector3{ x + v.x, y + v.y, z + v.z };
		}

		Vector3 operator + (const Vector3& v) const { return Vector3{ x + v.x, y + v.y, z + v.z }; }
		Vector3 operator - (const Vector3& v) const { return Vector3{ x - v.x, y - v.y, z - v.z }; }
		Vector3 operator * (const Vector3& v) const { return Vector3{ x * v.x, y * v.y, z * v.z }; }
		Vector3 operator / (const Vector3& v) const { return Vector3{ x / v.x, y / v.y, z / v.z }; }

		Vector3 operator + (float v) const { return Vector3{ x + v, y + v, z + v }; }
		Vector3 operator - (float v) const { return Vector3{ x - v, y - v, z - v }; } // Fixed: z - z bug
		Vector3 operator * (float v) const { return Vector3{ x * v, y * v, z * v }; }
		Vector3 operator / (float v) const { return Vector3{ x / v, y / v, z / v }; }

		Vector3& operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vector3& operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; } // Fixed: += bug
		Vector3& operator *= (const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; } // Fixed: y*=y bug
		Vector3& operator /= (const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; } // Fixed: y*=y bug

		Vector3& operator += (float v) { x += v; y += v; z += v; return *this; }
		Vector3& operator -= (float v) { x -= v; y -= v; z -= v; return *this; }
		Vector3& operator *= (float v) { x *= v; y *= v; z *= v; return *this; }
		Vector3& operator /= (float v) { x /= v; y /= v; z /= v; return *this; } // Fixed: y/=v bug

		float LengthSqr() const { return (x * x) + (y * y) + (z * z); } // Fixed: (x + x) bug
		float Length() const { return std::sqrt(LengthSqr()); }

		Vector3 Normalized() const {
			float len = Length();
			return len == 0.0f ? Vector3{ 0.0f } : (*this) / len;
		}

		float Dot(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }
		float Angle() const { return std::atan2(y, x); }
		float AngleZ() const { return std::atan2(y, z); }
		float AngleBetween(const Vector3& v) const { return std::acos(Dot(v.Normalized())); }
	};

	using Color = Vector3;
}