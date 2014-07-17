#pragma once
/*
Class: Vector3
Author: Callan White
Modified: 13/07/14
Description:
A template class for handling three dimensional vectors
of varying numeric data types. Heavy use of both the
auto and decltype keywords has been employed in an
attempt to mimick the type promotion (widening) mechanics
of primitive types.

16/07/14	Added cross product member function

15/07/14 -	Class rewritten to follow the style of the Vector
			2 class with respect to type promotion.

13/07/14 -	Initial version
*/

#include <iostream>

template <typename T>
class Vector3 {
public:
	T x;
	T y;
	T z;

	Vector3<T>() : x(0), y(0), z(0) {}
	Vector3<T>(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

	template <typename U>
	Vector3(const Vector3<U>& v) {
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}



	/*Compound arithmetic for rhs=Vector3*/
	template <typename U>
	Vector3<T>& operator+=(const Vector3<U>& v1) {
		x += static_cast<T>(v1.x);
		y += static_cast<T>(v1.y);
		z += static_cast<T>(v1.z);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator-=(const Vector3<U>& v1) {
		x -= static_cast<T>(v1.x);
		y -= static_cast<T>(v1.y);
		z -= static_cast<T>(v1.z);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator*=(const Vector3<U>& v1) {
		x *= static_cast<T>(v1.x);
		y *= static_cast<T>(v1.y);
		z *= static_cast<T>(v1.z);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator/=(const Vector3<U>& v1) {
		x /= static_cast<T>(v1.x);
		y /= static_cast<T>(v1.y);
		z /= static_cast<T>(v1.z);
		return *this;
	}

	/*Compound arithmetic for rhs=numeric*/
	template <typename U>
	Vector3<T>& operator+=(const U s) {
		x += static_cast<T>(s);
		y += static_cast<T>(s);
		z += static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator-=(const U s) {
		x -= static_cast<T>(s);
		y -= static_cast<T>(s);
		z -= static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator*=(const U s) {
		x *= static_cast<T>(s);
		y *= static_cast<T>(s);
		z *= static_cast<T>(s);
		return *this;
	}
	template <typename U>
	Vector3<T>& operator/=(const U s) {
		x /= static_cast<T>(s);
		y /= static_cast<T>(s);
		z /= static_cast<T>(s);
		return *this;
	}

	
	/*Dot product with type promotion*/
	template <typename U>
	auto Dot(const Vector3<U>& v) const -> decltype(x * v.x) {
		return x * v.x + y * v.y + z * v.z;
	}

	/*Cross product with type promotion*/
	template <typename U>
	auto Cross(const Vector3<U>& v) const -> decltype(x * v.x) {
		return Vector<decltype(x * v.x)>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	void Invert() {
		x = -x;
		y = -y;
		z = -z;
	}

	Vector3<T> Inverse() const {
		return -*this;
	}

	void ToZero() {
		x = y = z = 0;
	}

	/*Length function with type promotion*/
	auto Length() const -> decltype(sqrt(x)) {
		return sqrt(x * x + y * y + z * z);
	}

	T SquaredLength() const {
		return x * x + y * y + z * z;
	}

	/*Normal function with type promotion*/
	auto Normal() const -> Vector3<decltype(sqrt(x))> {
		Vector3<decltype(sqrt(x))> ret(*this);
		auto length = Length();
		if (length != 0) {
			length = 1 / length;
			ret *= length;
		}
		return ret;
	}

	T ComponentSum() const {
		return x + y + z;
	}

	/*Distance function with type promotion*/
	template <typename U>
	auto DistanceTo(const Vector3<U>& v) const -> decltype(sqrt(x - v.x)) {
		return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
	}

	/*SquaredDistanceTo function with type promotion*/
	template <typename U>
	auto SquaredDistanceTo(const Vector3<U>& v) const -> decltype(x - v.x) {
		return static_cast<decltype(x - v.x)>(pow(x - v.x, 2) + pow(y - v.y, 2) + pow(z - v.z, 2));
	}

	/*Floor function returns the calling type always*/
	Vector3<T> Floor() const {
		return Vector3<T>(static_cast<T>(floor(x)), static_cast<T>(floor(y)), static_cast<T>(floor(z)));
	}
};

/*ostream operator overload*/

template <typename T>
std::ostream& operator<<(std::ostream& o, const Vector3<T>&v) {
	o << "Vector3(" << v.x << "," << v.y << "," << v.z << ")" << std::endl;
	return o;
}

/*Boolean equality operator overloads*/

template <typename T>
bool operator==(const Vector3<T>& v, const Vector3<T>& v1) {
	return v.x == v1.x && v.y == v1.y && v.z == v1.z;
}

template <typename T>
bool operator!=(const Vector3<T>& v, const Vector3<T>& v1) {
	return !operator==(v,v1);
}

/*Addition binary arithmetic overloads*/

template <typename T, typename U>
auto operator+(const Vector3<T>& v, const U s) -> Vector3<decltype(v.x + s)> {
	return Vector3<decltype(v.x + s)>(v.x + s, v.y + s, v.z + s);
}
template <typename T, typename U>
auto operator+(const T s, const Vector3<U>& v) -> Vector3<decltype(v.x + s)> {
	return Vector3<decltype(v.x + s)>(v.x + s, v.y + s, v.z + s);
}
template <typename T, typename U>
auto operator+(const Vector3<T>& v, const Vector3<U>& v1) -> Vector3<decltype(v.x + v1.x)> {
	return Vector3<decltype(v.x + v1.x)>(v.x + v1.x, v.y + v1.y, v.z + v1.z);
}

/*Subtraction binary arithmetic overloads*/

template <typename T>
Vector3<T> operator-(const Vector3<T> & v) {
	return Vector3<T>(-v.x, -v.y, -v.z);
}
template <typename T, typename U>
auto operator-(const Vector3<T>& v, const Vector3<U>& v1) -> Vector3<decltype(v.x - v1.x)> {
	return Vector3<decltype(v.x - v1.x)>(v.x - v1.x, v.y - v1.y, v.z - v1.z);
}
template <typename T, typename U>
auto operator-(const Vector3<T>& v, const U s) -> Vector3<decltype(v.x - s)> {
	return Vector3<decltype(v.x - s)>(v.x - s, v.y - s, v.z - s);
}

/*Multiplication binary arithmetic overloads*/

template <typename T, typename U>
auto operator*(const Vector3<T>& v, const Vector3<U>& v1) -> Vector3<decltype(v.x * v1.x)> {
	return Vector3<decltype(v.x * v1.x)>(v.x * v1.x, v.y * v1.y, v.z * v1.z);
}
template <typename T, typename U>
auto operator*(const Vector3<T>& v, const U s) -> Vector3<decltype(v.x * s)> {
	return Vector3<decltype(v.x * s)>(v.x * s, v.y * s, v.z * s);
}
template <typename T, typename U>
auto operator*(const T s, const Vector3<U>& v) -> Vector3<decltype(v.x * s)> {
	return Vector3<decltype(v.x * s)>(v.x * s, v.y * s, v.z * s);
}

/*Division binary arithmetic overloads*/

template <typename T, typename U>
auto operator/(const Vector3<T>& v, const Vector3<U>& v1) -> Vector3<decltype(v.x / v1.x)> {
	return Vector3<decltype(v.x / v1.x)>(v.x / v1.x, v.y / v1.y, v.z / v1.z);
}
template <typename T, typename U>
auto operator/(const Vector3<T>& v, const U s) -> Vector3<decltype(v.x / s)> {
	return Vector3<decltype(v.x / s)>(v.x / s, v.y / s, v.z / s);
}

typedef Vector3<int> Vector3i;
typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<unsigned int> Vector3u;