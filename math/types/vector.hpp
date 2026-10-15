#pragma once
#include <cmath>
#include "vmatrix.hpp"
#include <algorithm>
#include <numbers>
struct Vector4D
{
	constexpr Vector4D(const float x = 0.0f, const float y = 0.0f,
					   const float z = 0.0f, const float w = 0.0f)
		: x(x), y(y), z(z), w(w)
	{}

	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};
class Vector
{
public:
	Vector operator+(const Vector& rhs) const noexcept
	{
		return Vector{ this->x + rhs.x, this->y + rhs.y, this->z + rhs.z };
	}

	Vector operator-(const Vector& rhs) const noexcept
	{
		return Vector{ this->x - rhs.x, this->y - rhs.y, this->z - rhs.z };
	}

	Vector operator*(const float rhs) const noexcept
	{
		return Vector{ this->x * rhs, this->y * rhs, this->z * rhs };
	}

	Vector operator/(const float rhs) const noexcept
	{
		return Vector{ this->x / rhs, this->y / rhs, this->z / rhs };
	}

	float DistToSquared(const Vector& rhs) const noexcept
	{
		const float _x = (this->x - rhs.x);
		const float _y = (this->y - rhs.y);
		const float _z = (this->z - rhs.z);

		return _x * _x + _y * _y + _z * _z;
	}

	float DistToSquaredInMeters(const Vector& rhs) const noexcept
	{
		 
		 

		return DistToSquared(rhs) * 0.00064516f;
	}

	Vector ToAngle()
	{
		return Vector{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	inline float Length_SQR_2D( ) const
	{
		return this->x * this->x + this->y * this->y;
	}
	inline float Length_2D( ) const
	{
		return std::sqrtf(Length_SQR_2D( ));
	}
	void Clamp( )
	{
		this->x = std::clamp(this->x, -89.0f, 89.0f);
		this->y = std::clamp(std::remainder(this->y, 360.0f), -180.0f, 180.0f);
		this->z = std::clamp(this->z, -50.0f, 50.0f);
	}
	void NormalizeInPlace( )
	{
		Vector& v = *this;

		float iradius = 1.0f / (this->Length( ) + FLT_EPSILON);

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;
	}

	Vector& Normalize()
	{
		this->x = std::remainderf(this->x, 360.f);
		this->y = std::remainderf(this->y, 360.f);
		this->z = std::remainderf(this->z, 360.f);
		return *this;
	}

	float Dot(const Vector& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}
	float Length( ) const
	{
		return sqrt(x * x + y * y + z * z);
	}
	constexpr const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}
	float x, y, z;
};
