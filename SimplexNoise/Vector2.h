#pragma once
/*
Class: Vector2
Author: Callan White
Modified: 13/07/14
Description:
A template class for handling two dimensional vectors
of varying numeric data types. Heavy use of both the
auto and decltype keywords has been employed in an
attempt to mimick the type promotion (widening) mechanics
of primitive types.

15/07/14 - Rewrote some member methods to utilise automatic type promotion
		   where necessary, and fixed some incorrect non-member operator 
		   functions. Also: const, const everywhere.

14/07/14 - Rewrote operator overloads to utilise C++11 auto and decltype
		   keywords in order to provide automatic type promotion

13/07/14 - Initial version
*/

#include <iostream>

template <typename T>
class Vector2 {
public:
	T x;
	T y;

	Vector2<T>() : x(0), y(0) {}
	Vector2<T>(T X, T Y) : x(X), y(Y) {}

	template <typename U>
	Vector2(const Vector2<U>& v) {
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
	}



	/*Compound arithmetic for rhs=Vector2*/
	template <typename U>
	Vector2<T>& operator+=(const Vector2<U>& v1) {
		x += static_cast<T>(v1.x);
		y += static_cast<T>(v1.y);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator-=(const Vector2<U>& v1) {
		x -= static_cast<T>(v1.x);
		y -= static_cast<T>(v1.y);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator*=(const Vector2<U>& v1) {
		x *= static_cast<T>(v1.x);
		y *= static_cast<T>(v1.y);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator/=(const Vector2<U>& v1) {
		x /= static_cast<T>(v1.x);
		y /= static_cast<T>(v1.y);
		return *this;
	}

	/*Compound arithmetic for rhs=numeric*/
	template <typename U>
	Vector2<T>& operator+=(const U s) {
		x += static_cast<T>(s);
		y += static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator-=(const U s) {
		x -= static_cast<T>(s);
		y -= static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator*=(const U s) {
		x *= static_cast<T>(s);
		y *= static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector2<T>& operator/=(const U s) {
		x /= static_cast<T>(s);
		y /= static_cast<T>(s);
		return *this;
	}

	
	/*Dot product with type promotion*/
	template <typename U>
	auto Dot(const Vector2<U>& v) const -> decltype(x * v.x) {
		return x * v.x + y * v.y;
	}

	void Invert() {
		x = -x;
		y = -y;
	}

	Vector2<T> Inverse() const {
		return -*this;
	}

	void ToZero() {
		x = y = 0;
	}

	/*Length function with type promotion*/
	auto Length() const -> decltype(sqrt(x)) {
		return sqrt(x * x + y * y);
	}

	T SquaredLength() const {
		return x * x + y * y;
	}

	/*Normal function with type promotion*/
	auto Normal() const -> Vector2<decltype(sqrt(x))> {
		Vector2<decltype(sqrt(x))> ret(*this);
		auto length = Length();
		if (length != 0) {
			length = 1 / length;
			ret *= length;
		}
		return ret;
	}

	T ComponentSum() const {
		return x + y;
	}

	/*Distance function with type promotion*/
	template <typename U>
	auto DistanceTo(const Vector2<U>& v) const -> decltype(sqrt(x - v.x)) {
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2));
	}

	/*SquaredDistanceTo function with type promotion*/
	template <typename U>
	auto SquaredDistanceTo(const Vector2<U>& v) const -> decltype(x - v.x) {
		return static_cast<decltype(x - v.x)>(pow(x - v.x, 2) + pow(y - v.y, 2));
	}

	/*Floor function (no cast to calling type provided as it only really applies
	to int, in which case a double should be expected similar to the cmath floor
	function)*/
	Vector2<T> Floor() const {
		return Vector2<T>(floor(x), floor(y));
	}
};

/*ostream operator overload*/

template <typename T>
std::ostream& operator<<(std::ostream& o, const Vector2<T>&v) {
	o << "Vector2(" << v.x << "," << v.y << ")" << std::endl;
	return o;
}

/*Boolean equality operator overloads*/

template <typename T>
bool operator==(const Vector2<T>& v, const Vector2<T>& v1) {
	return v.x == v1.x && v.y == v1.y;
}

template <typename T>
bool operator!=(const Vector2<T>& v, const Vector2<T>& v1) {
	return !operator==(v,v1);
}

/*Addition binary arithmetic overloads*/

template <typename T, typename U>
auto operator+(const Vector2<T>& v, const U s) -> Vector2<decltype(v.x + s)> {
	return Vector2<decltype(v.x + s)>(v.x + s, v.y + s);
}
template <typename T, typename U>
auto operator+(const T s, const Vector2<U>& v) -> Vector2<decltype(v.x + s)> {
	return Vector2<decltype(v.x + s)>(v.x + s, v.y + s);
}
template <typename T, typename U>
auto operator+(const Vector2<T>& v, const Vector2<U>& v1) -> Vector2<decltype(v.x + v1.x)> {
	return Vector2<decltype(v.x + v1.x)>(v.x + v1.x, v.y + v1.y);
}

/*Subtraction binary arithmetic overloads*/

template <typename T>
Vector2<T> operator-(const Vector2<T> & v) {
	return Vector2<T>(-v.x, -v.y);
}
template <typename T, typename U>
auto operator-(const Vector2<T>& v, const Vector2<U>& v1) -> Vector2<decltype(v.x - v1.x)> {
	return Vector2<decltype(v.x - v1.x)>(v.x - v1.x, v.y - v1.y);
}
template <typename T, typename U>
auto operator-(const Vector2<T>& v, const U s) -> Vector2<decltype(v.x - s)> {
	return Vector2<decltype(v.x - s)>(v.x - s, v.y - s);
}

/*Multiplication binary arithmetic overloads*/

template <typename T, typename U>
auto operator*(const Vector2<T>& v, const Vector2<U>& v1) -> Vector2<decltype(v.x * v1.x)> {
	return Vector2<decltype(v.x * v1.x)>(v.x * v1.x, v.y * v1.y);
}
template <typename T, typename U>
auto operator*(const Vector2<T>& v, const U s) -> Vector2<decltype(v.x * s)> {
	return Vector2<decltype(v.x * s)>(v.x * s, v.y * s);
}
template <typename T, typename U>
auto operator*(const T s, const Vector2<U>& v) -> Vector2<decltype(v.x * s)> {
	return Vector2<decltype(v.x * s)>(v.x * s, v.y * s);
}

/*Division binary arithmetic overloads*/

template <typename T, typename U>
auto operator/(const Vector2<T>& v, const Vector2<U>& v1) -> Vector2<decltype(v.x / v1.x)> {
	return Vector2<decltype(v.x / v1.x)>(v.x / v1.x, v.y / v1.y);
}
template <typename T, typename U>
auto operator/(const Vector2<T>& v, const U s) -> Vector2<decltype(v.x / s)> {
	return Vector2<decltype(v.x / s)>(v.x / s, v.y / s);
}

typedef Vector2<int> Vector2i;
typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;
typedef Vector2<unsigned int> Vector2u;