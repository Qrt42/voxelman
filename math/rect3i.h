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

#ifndef RECT3I_H
#define RECT3I_H

#include "vector3i.h"
#include <core/variant.h>

class Rect3i {

public:
	Vector3i pos;
	Vector3i size;

	Rect3i() {}

	Rect3i(Vector3i p_pos, Vector3i p_size) :
			pos(p_pos),
			size(p_size) {}

	Rect3i(const Rect3i &other) :
			pos(other.pos),
			size(other.size) {}

	static inline Rect3i from_center_extents(Vector3i center, Vector3i extents) {
		return Rect3i(center - extents, 2 * extents);
	}

	static inline Rect3i get_bounding_box(Rect3i a, Rect3i b) {

		Rect3i box;

		box.pos.x = MIN(a.pos.x, b.pos.x);
		box.pos.y = MIN(a.pos.y, b.pos.y);
		box.pos.z = MIN(a.pos.z, b.pos.z);

		Vector3i max_a = a.pos + a.size;
		Vector3i max_b = b.pos + b.size;

		box.size.x = MAX(max_a.x, max_b.x) - box.pos.x;
		box.size.y = MAX(max_a.y, max_b.y) - box.pos.y;
		box.size.z = MAX(max_a.z, max_b.z) - box.pos.z;

		return box;
	}

	bool inline contains(Vector3i p_pos) const {
		Vector3i end = pos + size;
		return p_pos.x >= pos.x &&
			   p_pos.y >= pos.y &&
			   p_pos.z >= pos.z &&
			   p_pos.x < end.x &&
			   p_pos.y < end.y &&
			   p_pos.z < end.z;
	}

	String to_string() const {
		return String("(o:{0}, s:{1})").format(varray(pos.to_vec3(), size.to_vec3()));
	}

	bool intersects(Rect3i other) {
		if (pos.x > other.pos.x + other.size.x)
			return false;
		if (pos.y > other.pos.y + other.size.y)
			return false;
		if (pos.z > other.pos.z + other.size.z)
			return false;
		if (other.pos.x > pos.x + size.x)
			return false;
		if (other.pos.y > pos.y + size.y)
			return false;
		if (other.pos.z > pos.z + size.z)
			return false;
		return true;
	}
};

inline bool operator!=(const Rect3i &a, const Rect3i &b) {
	return a.pos != b.pos || a.size != b.size;
}

#endif // RECT3I_H
