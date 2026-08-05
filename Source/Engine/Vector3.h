#pragma once
#include <cmath>
#include <cassert>
namespace nu
{
	struct Vector3
	{
		float x, y,z;

		Vector3() = default;
		Vector3(float x, float y,float z) : x{ x }, y{ y }, z{ z } {};
		Vector3(float v) : x{ v }, y{ v }, z{ v } {};

		float operator [] (unsigned int i) const {

			assert(i < 3);return  (&x)[i];

		}
		float& operator [] (unsigned int i) {

			assert(i < 3);return  (&x)[i];

		}
		bool operator == (const Vector3& v) const { return this->x == v.x && this->y == v.y && this->z == v.z; }
		bool operator != (const Vector3& v) const { return this->x != v.x && this->y != v.y && this->z != v.z; }



		Vector3 Add(Vector3 v) {
			return Vector3{ this->x + v.x,this->y + v.y,this->z+v.z };
		}

		Vector3 operator + (const Vector3& v) const
		{
			return  Vector3{ this->x + v.x,this->y + v.y,this->z + v.z };
		}
		Vector3 operator - (const Vector3& v) const
		{
			return  Vector3{ this->x - v.x,this->y - v.y,this->z - v.z };
		}
		Vector3 operator * (const Vector3& v) const
		{
			return  Vector3{ this->x * v.x,this->y * v.y,this->z * v.z };
		}
		Vector3 operator / (const Vector3& v) const
		{
			return  Vector3{ this->x / v.x,this->y / v.y,this->z / v.z };
		}




		///////
		Vector3 operator + (float v) const { return  Vector3{ this->x + v,this->y + v,this->z + v }; }
		Vector3 operator - (float v) const
		{
			return  Vector3{ this->x - v,this->y - v,this->z - z };
		}
		Vector3 operator * (float v) const
		{
			return  Vector3{ this->x * v,this->y * v,this->z * v };
		}
		Vector3 operator / (float v) const
		{
			return  Vector3{ this->x / v,this->y / v,this->z / v };
		}

		Vector3 operator += (const Vector3& v)
		{
			Vector3{ this->x += v.x,this->y += v.y,this->z += v.z };
			return *this;
		}
		Vector3 operator -= (const Vector3& v)
		{
			Vector3{ this->x += v.x,this->y += v.y,this->z += v.z };
			return *this;
		}
		Vector3 operator *= (const Vector3& v)
		{
			Vector3{ this->x *= v.x,this->y *= v.y,this->y *= v.y };
			return *this;
		}
		Vector3 operator /= (const Vector3& v)
		{
			Vector3{ this->x /= v.x,this->y /= v.y,this->y /= v.y };
			return *this;
		}
		////////
		Vector3 operator += (float v)
		{
			Vector3{ this->x += v,this->y += v,this->z += v };
			return *this;
		}
		Vector3 operator -= (float v)
		{
			Vector3{ this->x -= v,this->y -= v,this->z -= v };
			return *this;
		}
		Vector3 operator *= (float v)
		{
			Vector3{ this->x *= v,this->y *= v,this->z *= v };
			return *this;
		}
		Vector3 operator /= (float v)
		{
			Vector3{ this->x /= v,this->y /= v,this->y /= v };
			return *this;
		}


		float LengthSqr() const { return (x + x) + (y * y) + (z * z); }
		float Length() const { return (std::sqrt(LengthSqr())); }

		Vector3 Normalized() const { return(*this) / Length(); }
		float Dot(const Vector3& v) { return(this->x * v.x) + (this->y * v.y) + (this->z * v.z); }
		float Angle() { return std::atan2(this->y, this->x); }
		float AngleZ() { return std::atan2(this->y, this->z); }
		float AngleBetween(const Vector3& v) { return std::acos(Dot(v)); }
		Vector3 Rotate(float radians) const
		{
			Vector3 v;
			v.x = x * std::cos(radians) - y * std::sin(radians) - z * std::sin(radians);
			v.y = x * std::sin(radians) + y * std::cos(radians) - z * std::sin(radians);
			v.z = -(x * std::sin(radians)) - y * std::cos(radians) + z * std::sin(radians);

			return v;
		}
		using Color = Vector3;


	};
	using Color = Vector3;
}
