#pragma once
#include <cmath>
#include <cassert>
namespace nu
{
	struct Vector2
	{ 
		float x, y;

		Vector2() = default;
		Vector2(float x, float y) : x{ x }, y{ y } {};
		Vector2(float v) : x{ v }, y{ v } {};

		float operator [] (unsigned int i) const{

			assert(i < 2);return  (&x)[i];

		}
		float& operator [] (unsigned int i) {

			assert(i < 2);return  (&x)[i];

		}
		bool operator == (const Vector2& v) const { return this->x == v.x && this->y == v.y; }
		bool operator != (const Vector2& v) const { return this->x != v.x && this->y != v.y; }



		Vector2 Add(Vector2 v) {
			return Vector2{ this->x + v.x,this->y + v.y };
		}

		Vector2 operator + (const Vector2& v) const
		{ 
			return  Vector2{ this->x + v.x,this->y + v.y };
		}
		Vector2 operator - (const Vector2& v) const
		{
			return  Vector2{ this->x - v.x,this->y - v.y };
		}
		Vector2 operator * (const Vector2& v) const
		{
			return  Vector2{ this->x * v.x,this->y * v.y };
		}
		Vector2 operator / (const Vector2& v) const
		{
			return  Vector2{ this->x / v.x,this->y / v.y };
		}

		
		

		///////
		Vector2 operator + (float v) const{	return  Vector2{ this->x + v,this->y + v };}
		Vector2 operator - (float v) const
		{
			return  Vector2{ this->x - v,this->y - v };
		}
		Vector2 operator * (float v) const
		{
			return  Vector2{ this->x * v,this->y * v };
		}
		Vector2 operator / (float v) const
		{
			return  Vector2{ this->x / v,this->y / v };
		}

		Vector2 operator += (const Vector2& v)
		{
			Vector2{ this->x += v.x,this->y += v.y };
			return *this;
		}
		Vector2 operator -= (const Vector2& v)
		{
			Vector2{ this->x -= v.x,this->y -= v.y };
			return *this;
		}
		Vector2 operator *= (const Vector2& v)
		{
			Vector2{ this->x *= v.x,this->y *= v.y };
			return *this;
		}
		Vector2 operator /= (const Vector2& v)
		{
			Vector2{ this->x /= v.x,this->y /=	 v.y };
			return *this;
		}
		////////
		Vector2 operator += (float v)
		{
			Vector2{ this->x += v,this->y += v };
			return *this;
		}
		Vector2 operator -= (float v)
		{
			Vector2{ this->x -= v,this->y -= v };
			return *this;
		}
		Vector2 operator *= (float v)
		{
			Vector2{ this->x *= v,this->y *=v};
			return *this;
		}
		Vector2 operator /= (float v)
		{
			Vector2{ this->x /= v,this->y /= v };
			return *this;
		}


		float LengthSqr() const { return x * x + y * y; }
		float Length() const { return std::sqrt(x * x + y * y); }

		Vector2 Normalized() const { return(*this) / Length(); }
		float Dot(const Vector2& v) { return(this->x * v.x) + (this->y * v.y); }
		float Angle() { return std::atan2(this->y,this->x); }
		float AngleBetween(const Vector2& v) { return std::acos(Dot(v)); }
		Vector2 Rotate(float radians) const
		{
			Vector2 v;
			v.x = this->x * std::cos(radians) - this->y * std::sin(radians); 
			v.y = this->x * std::sin(radians) + this->y * std::cos(radians);

			return v;
		}
		


	};
}