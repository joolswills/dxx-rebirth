/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Header file for vector/matrix library
 *
 */

#pragma once

#ifdef __cplusplus
#include <cassert>
#include <cstdint>
#include <utility>
#include "fwd-vecmat.h"

namespace dcx {

//The basic fixed-point vector.  Access elements by name or position
struct vms_vector
{
	fix x, y, z;
};

class vm_distance
{
public:
	fix d;
	/* Default constructor only required because Fcd_cache,SoundObjects
	 * have global scope instances of vm_distance.  They should be
	 * converted to construct as needed, then the default constructor
	 * should be removed.
	 */
	constexpr vm_distance() : d(0) {}
	constexpr explicit vm_distance(const fix &f) :
		d(f)
	{
	}
	template <typename T>
		vm_distance &operator+=(const T &rhs)
		{
			return *this = (*this + rhs);
		}
	template <typename T>
		vm_distance &operator*=(const T &rhs)
		{
			return *this = (*this * rhs);
		}
	template <typename T>
		vm_distance &operator/=(const T &rhs)
		{
			return *this = (*this / rhs);
		}
	constexpr vm_distance operator+(const vm_distance &rhs) const
	{
		return vm_distance{d + rhs.d};
	}
	constexpr vm_distance operator*(const int &f) const
	{
		return vm_distance{d * f};
	}
	constexpr vm_distance operator/(const int &f) const
	{
		return vm_distance{d / f};
	}
	constexpr bool operator<(const fix &f) const
	{
		return d < f;
	}
	constexpr bool operator<(const vm_distance &rhs) const
	{
		return d < rhs.d;
	}
	/* Clang chokes using a template operator> to swap the arguments.
	 * Use a non-template operator> for the one needed type.
	 */
	constexpr bool operator>(const fix &f) const
	{
		return d > f;
	}
	template <typename T>
		bool operator>(const T &t) const = delete;
	constexpr explicit operator bool() const { return d; }
	template <typename T>
		operator T() const = delete;
	constexpr operator fix() const
	{
		return d;
	}
	static constexpr vm_distance maximum_value()
	{
		return vm_distance{0x7fffffff};
	}
	static constexpr vm_distance minimum_value()
	{
		return vm_distance{0};
	}
	constexpr vm_distance_squared operator*(const vm_distance &) const;
};

class vm_magnitude : public vm_distance
{
public:
	constexpr explicit vm_magnitude(const uint32_t &f) :
		vm_distance(f)
	{
	}
};

class vm_distance_squared
{
public:
	fix64 d2;
	constexpr explicit vm_distance_squared(const fix64 &f2) :
		d2(f2)
	{
	}
	constexpr bool operator<(const vm_distance_squared &rhs) const
	{
		return d2 < rhs.d2;
	}
	constexpr bool operator>(const vm_distance_squared &rhs) const
	{
		return d2 > rhs.d2;
	}
	constexpr bool operator>=(const vm_distance_squared &rhs) const
	{
		return !(*this < rhs);
	}
	template <typename T>
		vm_distance_squared &operator-=(const T &rhs)
		{
			return *this = (*this - rhs);
		}
	constexpr vm_distance_squared operator-(const fix &) const = delete;
	constexpr vm_distance_squared operator-(const fix64 &f2) const
	{
		return vm_distance_squared{d2 - f2};
	}
	explicit operator bool() const { return d2; }
	template <typename T>
		constexpr operator T() const = delete;
	constexpr operator fix64() const
	{
		return d2;
	}
	static constexpr vm_distance_squared maximum_value()
	{
		return vm_distance_squared{0x7fffffffffffffff};
	}
	static constexpr vm_distance_squared minimum_value()
	{
		return vm_distance_squared{0};
	}
};

class vm_magnitude_squared : public vm_distance_squared
{
public:
	constexpr explicit vm_magnitude_squared(const uint64_t &f2) :
		vm_distance_squared(f2)
	{
	}
};

constexpr vm_distance_squared vm_distance::operator*(const vm_distance &rhs) const
{
	return vm_distance_squared{static_cast<fix64>(static_cast<fix>(*this)) * static_cast<fix64>(static_cast<fix>(rhs))};
}

#define DEFINE_SERIAL_VMS_VECTOR_TO_MESSAGE()	\
	DEFINE_SERIAL_UDT_TO_MESSAGE(vms_vector, v, (v.x, v.y, v.z));	\
	ASSERT_SERIAL_UDT_MESSAGE_SIZE(vms_vector, 12)

//Angle vector.  Used to store orientations
struct vms_angvec
{
	fixang p, b, h;
};


//A 3x3 rotation matrix.  Sorry about the numbering starting with one.
//Ordering is across then down, so <m1,m2,m3> is the first row
struct vms_matrix
{
	vms_vector rvec, uvec, fvec;
};

// Quaternion structure
struct vms_quaternion
{
    signed short w, x, y, z;
};


//Macros/functions to fill in fields of structures

//macro to set a vector to zero.  we could do this with an in-line assembly
//macro, but it's probably better to let the compiler optimize it.
//Note: NO RETURN VALUE
static inline void vm_vec_zero(vms_vector &v)
{
	v = {};
}

//macro set set a matrix to the identity. Note: NO RETURN VALUE

// DPH (18/9/98): Begin mod to fix linefeed problem under linux. Uses an
// inline function instead of a multi-line macro to fix CR/LF problems.

// DPH (19/8/98): End changes.

//Global constants

//Here's a handy constant

//negate a vector
static inline void vm_vec_negate(vms_vector &v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
}

static inline vms_vector vm_vec_negated(vms_vector v) __attribute_warn_unused_result;
static inline vms_vector vm_vec_negated(vms_vector v)
{
	return vm_vec_negate(v), v;
}

//Functions in library

//adds two vectors, fills in dest, returns ptr to dest
//ok for dest to equal either source, but should use vm_vec_add2() if so
static inline vms_vector vm_vec_add (const vms_vector &src0, const vms_vector &src1) __attribute_warn_unused_result;
static inline vms_vector vm_vec_add (const vms_vector &src0, const vms_vector &src1)
{
	vms_vector dest;
	return vm_vec_add(dest, src0, src1), dest;
}


//subs two vectors, fills in dest, returns ptr to dest
//ok for dest to equal either source, but should use vm_vec_sub2() if so
static inline vms_vector &vm_vec_sub(vms_vector &dest, const vms_vector &src0, const vms_vector &src1)
{
#ifdef DXX_CONSTANT_TRUE
	if (DXX_CONSTANT_TRUE(&src0 == &src1))
		DXX_ALWAYS_ERROR_FUNCTION(vm_vec_sub_same_op, "vm_vec_sub with &src0 == &src1");
	else if (DXX_CONSTANT_TRUE(src0.x == src1.x && src0.y == src1.y && src0.z == src1.z))
		DXX_ALWAYS_ERROR_FUNCTION(vm_vec_sub_same_values, "vm_vec_sub with equal value inputs");
#endif
	return _vm_vec_sub(dest, src0, src1);
}

__attribute_warn_unused_result
static inline vms_vector vm_vec_sub (const vms_vector &src0, const vms_vector &src1)
{
	vms_vector dest;
	return vm_vec_sub(dest, src0, src1), dest;
}

//averages two vectors. returns ptr to dest
//dest can equal either source
static inline vms_vector vm_vec_avg (const vms_vector &src0, const vms_vector &src1) __attribute_warn_unused_result;
static inline vms_vector vm_vec_avg (const vms_vector &src0, const vms_vector &src1)
{
	vms_vector dest;
	return vm_vec_avg(dest, src0, src1), dest;
}

//scales and copies a vector.  returns ptr to dest
#define vm_vec_copy_scale(A,B,...)	vm_vec_copy_scale(A, ## __VA_ARGS__, B)
static inline vms_vector vm_vec_copy_scale(vms_vector src, fix s) __attribute_warn_unused_result;
static inline vms_vector vm_vec_copy_scale(vms_vector src, fix s)
{
	return vm_vec_scale(src, s), src;
}

//scales a vector, adds it to another, and stores in a 3rd vector
//dest = src1 + k * src2
static inline vms_vector vm_vec_scale_add(const vms_vector &src1, const vms_vector &src2, fix k) __attribute_warn_unused_result;
static inline vms_vector vm_vec_scale_add(const vms_vector &src1, const vms_vector &src2, fix k)
{
	vms_vector dest;
	return vm_vec_scale_add(dest, src1, src2, k), dest;
}

static inline vms_vector vm_vec_normalized(vms_vector v) __attribute_warn_unused_result;
static inline vms_vector vm_vec_normalized(vms_vector v)
{
	return vm_vec_normalize(v), v;
}

static inline vms_vector vm_vec_normalized_quick(vms_vector v) __attribute_warn_unused_result;
static inline vms_vector vm_vec_normalized_quick(vms_vector v)
{
	return vm_vec_normalize_quick(v), v;
}

static inline vms_vector vm_vec_cross(const vms_vector &src0, const vms_vector &src1) __attribute_warn_unused_result;
static inline vms_vector vm_vec_cross(const vms_vector &src0, const vms_vector &src1)
{
	vms_vector dest;
	return vm_vec_cross(dest, src0, src1), dest;
}

static inline vms_vector vm_vec_normal(const vms_vector &p0, const vms_vector &p1, const vms_vector &p2) __attribute_warn_unused_result;
static inline vms_vector vm_vec_normal(const vms_vector &p0, const vms_vector &p1, const vms_vector &p2)
{
	vms_vector dest;
	return vm_vec_normal(dest, p0, p1, p2), dest;
}

static inline vms_vector vm_vec_perp (const vms_vector &p0, const vms_vector &p1, const vms_vector &p2) __attribute_warn_unused_result;
static inline vms_vector vm_vec_perp (const vms_vector &p0, const vms_vector &p1, const vms_vector &p2)
{
	vms_vector dest;
	return vm_vec_perp(dest, p0, p1, p2), dest;
}

static inline vms_matrix vm_angles_2_matrix (const vms_angvec &a) __attribute_warn_unused_result;
static inline vms_matrix vm_angles_2_matrix (const vms_angvec &a)
{
	vms_matrix m;
	return vm_angles_2_matrix(m, a), m;
}

static inline vms_matrix vm_vector_2_matrix (const vms_vector &fvec, const vms_vector *uvec, const vms_vector *rvec) __attribute_warn_unused_result;
static inline vms_matrix vm_vector_2_matrix (const vms_vector &fvec, const vms_vector *uvec, const vms_vector *rvec)
{
	vms_matrix m;
	return vm_vector_2_matrix(m, fvec, uvec, rvec), m;
}

static inline vms_vector vm_vec_rotate (const vms_vector &src, const vms_matrix &m) __attribute_warn_unused_result;
static inline vms_vector vm_vec_rotate (const vms_vector &src, const vms_matrix &m)
{
	vms_vector dest;
	return vm_vec_rotate(dest, src, m), dest;
}

//transpose a matrix in place. returns ptr to matrix
static inline void vm_transpose_matrix(vms_matrix &m)
{
	using std::swap;
	swap(m.uvec.x, m.rvec.y);
	swap(m.fvec.x, m.rvec.z);
	swap(m.fvec.y, m.uvec.z);
}

static inline vms_matrix vm_transposed_matrix(vms_matrix m) __attribute_warn_unused_result;
static inline vms_matrix vm_transposed_matrix(vms_matrix m)
{
	vm_transpose_matrix(m);
	return m;
}

//mulitply 2 matrices, fill in dest.  returns ptr to dest
static inline void vm_matrix_x_matrix(vms_matrix &dest, const vms_matrix &src0, const vms_matrix &src1)
{
#ifdef DXX_CONSTANT_TRUE
	if (DXX_CONSTANT_TRUE(&dest == &src0))
		DXX_ALWAYS_ERROR_FUNCTION(vm_matrix_x_matrix_dest_src0, "vm_matrix_x_matrix with &dest == &src0");
	else if (DXX_CONSTANT_TRUE(&dest == &src1))
		DXX_ALWAYS_ERROR_FUNCTION(vm_matrix_x_matrix_dest_src1, "vm_matrix_x_matrix with &dest == &src1");
#endif
	assert(&dest != &src0);
	assert(&dest != &src1);
	return _vm_matrix_x_matrix(dest, src0, src1);
}

static inline vms_matrix vm_matrix_x_matrix(const vms_matrix &src0, const vms_matrix &src1) __attribute_warn_unused_result;
static inline vms_matrix vm_matrix_x_matrix(const vms_matrix &src0, const vms_matrix &src1)
{
	vms_matrix dest;
	vm_matrix_x_matrix(dest, src0, src1);
	return dest;
}

static inline vms_angvec vm_extract_angles_matrix (const vms_matrix &m) __attribute_warn_unused_result;
static inline vms_angvec vm_extract_angles_matrix (const vms_matrix &m)
{
	vms_angvec a;
	return vm_extract_angles_matrix(a, m), a;
}

//fills in fields of an angle vector
static inline void vm_angvec_make(vms_angvec *v, fixang p, fixang b, fixang h)
{
	v->p = p;
	v->b = b;
	v->h = h;
}

}

#endif
