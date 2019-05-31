/**
* 
* Voxel Tools for Godot Engine
* 
* Copyright(c) 2016 Marc Gilleron
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#ifndef VOXEL_VECTOR3I_H
#define VOXEL_VECTOR3I_H

#include <core/hashfuncs.h>
#include <core/math/vector3.h>

struct Vector3i {

	union {
		struct {
			int x;
			int y;
			int z;
		};
		int coords[3];
	};

	_FORCE_INLINE_ Vector3i() :
			x(0),
			y(0),
			z(0) {}

	_FORCE_INLINE_ Vector3i(int xyz) :
			x(xyz),
			y(xyz),
			z(xyz) {}

	_FORCE_INLINE_ Vector3i(int px, int py, int pz) :
			x(px),
			y(py),
			z(pz) {}

	_FORCE_INLINE_ Vector3i(const Vector3i &other) {
		*this = other;
	}

	_FORCE_INLINE_ Vector3i(const Vector3 &f) {
		x = Math::floor(f.x);
		y = Math::floor(f.y);
		z = Math::floor(f.z);
	}

	_FORCE_INLINE_ Vector3 to_vec3() const {
		return Vector3(x, y, z);
	}

	_FORCE_INLINE_ int volume() const {
		return x * y * z;
	}

	_FORCE_INLINE_ int length_sq() const {
		return x * x + y * y + z * z;
	}

	_FORCE_INLINE_ real_t length() const {
		return Math::sqrt((real_t)length_sq());
	}

	_FORCE_INLINE_ int distance_sq(const Vector3i &other) const;

	_FORCE_INLINE_ Vector3i &operator=(const Vector3i &other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	_FORCE_INLINE_ void operator+=(const Vector3i &other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	_FORCE_INLINE_ void operator-=(const Vector3i &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	_FORCE_INLINE_ Vector3i operator-() const {
		return Vector3i(-x, -y, -z);
	}

	_FORCE_INLINE_ int &operator[](unsigned int i) {
		return coords[i];
	}

	void clamp_to(const Vector3i min, const Vector3i max) {
		if (x < min.x) x = min.x;
		if (y < min.y) y = min.y;
		if (z < min.z) z = min.z;

		// TODO Not sure it should clamp like that...
		if (x >= max.x) x = max.x - 1;
		if (y >= max.y) y = max.y - 1;
		if (z >= max.z) z = max.z - 1;
	}

	_FORCE_INLINE_ bool is_contained_in(const Vector3i &min, const Vector3i &max) {
		return x >= min.x && y >= min.y && z >= min.z && x < max.x && y < max.y && z < max.z;
	}

	_FORCE_INLINE_ Vector3i wrap(const Vector3i &size) {
		return Vector3i(
				x % size.x,
				y % size.y,
				z % size.z);
	}

	static void sort_min_max(Vector3i &a, Vector3i &b) {
		sort_min_max(a.x, b.x);
		sort_min_max(a.y, b.y);
		sort_min_max(a.z, b.z);
	}

private:
	static _FORCE_INLINE_ void sort_min_max(int &a, int &b) {
		if (a > b) {
			int temp = a;
			a = b;
			b = temp;
		}
	}
};

_FORCE_INLINE_ Vector3i operator+(const Vector3i a, const Vector3i &b) {
	return Vector3i(a.x + b.x, a.y + b.y, a.z + b.z);
}

_FORCE_INLINE_ Vector3i operator-(const Vector3i &a, const Vector3i &b) {
	return Vector3i(a.x - b.x, a.y - b.y, a.z - b.z);
}

_FORCE_INLINE_ Vector3i operator*(const Vector3i &a, const Vector3i &b) {
	return Vector3i(a.x * b.x, a.y * b.y, a.z * b.z);
}

_FORCE_INLINE_ Vector3i operator*(const Vector3i &a, int n) {
	return Vector3i(a.x * n, a.y * n, a.z * n);
}

_FORCE_INLINE_ Vector3i operator*(int n, const Vector3i &a) {
	return Vector3i(a.x * n, a.y * n, a.z * n);
}

_FORCE_INLINE_ Vector3i operator/(const Vector3i &a, int n) {
	return Vector3i(a.x / n, a.y / n, a.z / n);
}

_FORCE_INLINE_ bool operator==(const Vector3i &a, const Vector3i &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

_FORCE_INLINE_ bool operator!=(const Vector3i &a, const Vector3i &b) {
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

_FORCE_INLINE_ Vector3i operator<<(const Vector3i &a, int b) {
	return Vector3i(a.x << b, a.y << b, a.z << b);
}

_FORCE_INLINE_ Vector3i operator>>(const Vector3i &a, int b) {
	return Vector3i(a.x >> b, a.y >> b, a.z >> b);
}

_FORCE_INLINE_ bool operator<(const Vector3i &a, const Vector3i &b) {

	if (a.x == b.x) {
		if (a.y == b.y) {
			return a.z < b.z;
		} else {
			return a.y < b.y;
		}
	} else {
		return a.x < b.x;
	}
}

_FORCE_INLINE_ int Vector3i::distance_sq(const Vector3i &other) const {
	return (other - *this).length_sq();
}

struct Vector3iHasher {
	static _FORCE_INLINE_ uint32_t hash(const Vector3i &v) {
		uint32_t hash = hash_djb2_one_32(v.x);
		hash = hash_djb2_one_32(v.y, hash);
		return hash_djb2_one_32(v.z, hash);
	}
};

#endif // VOXEL_VECTOR3I_H
