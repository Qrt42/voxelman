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

#ifndef VOXEL_BUFFER_H
#define VOXEL_BUFFER_H

#include "../math/vector3i.h"
#include "core/reference.h"
#include "core/vector.h"

// Dense voxels data storage.
// Organized in 8-bit channels like images, all optional.
// Note: for float storage (marching cubes for example), you can map [0..256] to [0..1] and save 3 bytes per cell

class VoxelBuffer : public Reference {
	GDCLASS(VoxelBuffer, Reference)

public:
	enum ChannelId {
		CHANNEL_TYPE = 0,
		CHANNEL_ISOLEVEL,
		CHANNEL_LIGHT_COLOR_R,
		CHANNEL_LIGHT_COLOR_G,
		CHANNEL_LIGHT_COLOR_B,
        CHANNEL_AO,
		CHANNEL_RANDOM_AO,
		CHANNEL_LIQUID_TYPES,
		CHANNEL_LIQUID_FILL,
		CHANNEL_LIQUID_FLOW,
		// Arbitrary value, 8 should be enough. Tweak for your needs.
		MAX_CHANNELS
	};

	// TODO use C++17 inline to initialize right here...
	static const char *CHANNEL_ID_HINT_STRING;

	// TODO Quantification options
	//	enum ChannelFormat {
	//		FORMAT_I8_Q256U, // 0..255 integer
	//		FORMAT_F8_Q1S, // -1..1 float stored in 8 bits
	//		FORMAT_F16_Q128S // -128..128 float stored in 16 bits
	//	};

	// Converts -1..1 float into 0..255 integer
	static inline int iso_to_byte(real_t iso) {
		int v = static_cast<int>(128.f * iso + 128.f);
		if (v > 255)
			return 255;
		else if (v < 0)
			return 0;
		return v;
	}

	// Converts 0..255 integer into -1..1 float
	static inline real_t byte_to_iso(int b) {
		return static_cast<float>(b - 128) / 128.f;
	}

	_FORCE_INLINE_ int get_margin_start() const { return _margin_start; }
	_FORCE_INLINE_ int get_margin_end() const { return _margin_end; }

	void create(int sx, int sy, int sz, int margin_start = 0, int margin_end = 0);
	void clear();
	void clear_channel(unsigned int channel_index, int clear_value = 0);
	_FORCE_INLINE_ void clear_channel_f(unsigned int channel_index, float clear_value = 0) { clear_channel(channel_index, iso_to_byte(clear_value)); }

	_FORCE_INLINE_ const Vector3i &get_size() const { return _size; }
	_FORCE_INLINE_ const Vector3i &get_actual_size() const { return _actual_size; }

	void set_default_values(uint8_t values[MAX_CHANNELS]);

	int get_voxel(int x, int y, int z, unsigned int channel_index = 0) const;
	void set_voxel(int value, int x, int y, int z, unsigned int channel_index = 0);
	void set_voxel_v(int value, Vector3 pos, unsigned int channel_index = 0);

	void try_set_voxel(int x, int y, int z, int value, unsigned int channel_index = 0);

	_FORCE_INLINE_ void set_voxel_f(real_t value, int x, int y, int z, unsigned int channel_index = 0) { set_voxel(iso_to_byte(value), x, y, z, channel_index); }
	_FORCE_INLINE_ real_t get_voxel_f(int x, int y, int z, unsigned int channel_index = 0) const { return byte_to_iso(get_voxel(x, y, z, channel_index)); }

	_FORCE_INLINE_ int get_voxel(const Vector3i pos, unsigned int channel_index = 0) const { return get_voxel(pos.x, pos.y, pos.z, channel_index); }
	_FORCE_INLINE_ void set_voxel(int value, const Vector3i pos, unsigned int channel_index = 0) { set_voxel(value, pos.x, pos.y, pos.z, channel_index); }

	void fill(int defval, unsigned int channel_index = 0);
	_FORCE_INLINE_ void fill_f(float value, unsigned int channel = 0) { fill(iso_to_byte(value), channel); }
	void fill_area(int defval, Vector3i min, Vector3i max, unsigned int channel_index = 0);

	bool is_uniform(unsigned int channel_index) const;

	void compress_uniform_channels();

	void copy_from(const VoxelBuffer &other, unsigned int channel_index = 0);
	void copy_from(const VoxelBuffer &other, Vector3i src_min, Vector3i src_max, Vector3i dst_min, unsigned int channel_index = 0);

	_FORCE_INLINE_ bool validate_pos(unsigned int x, unsigned int y, unsigned int z) const {
		return x < _actual_size.x && y < _actual_size.y && z < _actual_size.z;
	}

	_FORCE_INLINE_ unsigned int index(unsigned int x, unsigned int y, unsigned int z) const {
		return y + _actual_size.y * (x + _actual_size.x * z);
	}

	//	_FORCE_INLINE_ unsigned int row_index(unsigned int x, unsigned int y, unsigned int z) const {
	//		return _actual_size.y * (x + _actual_size.x * z);
	//	}

	_FORCE_INLINE_ unsigned int get_volume() const {
		return _actual_size.x * _actual_size.y * _actual_size.z;
	}

	uint8_t *get_channel_raw(unsigned int channel_index) const;
    
    void generate_ao();

    void add_light(int local_x, int local_y, int local_z, int size, Color color);
	void clear_lights();

	VoxelBuffer();
	~VoxelBuffer();

private:
	void create_channel_noinit(int i, Vector3i size);
	void create_channel(int i, Vector3i size, uint8_t defval);
	void delete_channel(int i);

protected:
	static void _bind_methods();

	_FORCE_INLINE_ int get_size_x() const { return _size.x; }
	_FORCE_INLINE_ int get_size_y() const { return _size.y; }
	_FORCE_INLINE_ int get_size_z() const { return _size.z; }
	_FORCE_INLINE_ Vector3 _get_size_binding() const { return _size.to_vec3(); }

	_FORCE_INLINE_ int get_actual_size_x() const { return _actual_size.x; }
	_FORCE_INLINE_ int get_actual_size_y() const { return _actual_size.y; }
	_FORCE_INLINE_ int get_actual_size_z() const { return _actual_size.z; }
	_FORCE_INLINE_ Vector3 _get_actual_size_binding() const { return _actual_size.to_vec3(); }

	_FORCE_INLINE_ int _get_voxel_binding(int x, int y, int z, unsigned int channel) const { return get_voxel(x, y, z, channel); }
	_FORCE_INLINE_ void _set_voxel_binding(int value, int x, int y, int z, unsigned int channel) { set_voxel(value, x, y, z, channel); }
	void _copy_from_binding(Ref<VoxelBuffer> other, unsigned int channel);
	void _copy_from_area_binding(Ref<VoxelBuffer> other, Vector3 src_min, Vector3 src_max, Vector3 dst_min, unsigned int channel);
	_FORCE_INLINE_ void _fill_area_binding(int defval, Vector3 min, Vector3 max, unsigned int channel_index) { fill_area(defval, Vector3i(min), Vector3i(max), channel_index); }
	_FORCE_INLINE_ void _set_voxel_f_binding(real_t value, int x, int y, int z, unsigned int channel) { set_voxel_f(value, x, y, z, channel); }

private:
	struct Channel {
		// Allocated when the channel is populated.
		// Flat array, in order [z][x][y] because it allows faster vertical-wise access (the engine is Y-up).
		uint8_t *data;

		// Default value when data is null
		uint8_t defval;

		Channel() :
				data(NULL),
				defval(0) {}
	};

	// Each channel can store arbitary data.
	// For example, you can decide to store colors (R, G, B, A), gameplay types (type, state, light) or both.
	Channel _channels[MAX_CHANNELS];

	// How many voxels are there in the three directions. All populated channels have the same size.
	Vector3i _size;
	Vector3i _actual_size;

	uint32_t _margin_start;
	uint32_t _margin_end;
};

VARIANT_ENUM_CAST(VoxelBuffer::ChannelId)

#endif // VOXEL_BUFFER_H
