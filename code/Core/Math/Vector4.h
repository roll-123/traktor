/*
 * TRAKTOR
 * Copyright (c) 2022-2024 Anders Pistol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "Core/Config.h"
#include "Core/Math/Const.h"
#include "Core/Math/MathConfig.h"
#include "Core/Math/MathUtils.h"
#include "Core/Math/Scalar.h"

// import/export mechanism.
#undef T_DLLCLASS
#if defined(T_CORE_EXPORT)
#	define T_DLLCLASS T_DLLEXPORT
#else
#	define T_DLLCLASS T_DLLIMPORT
#endif

namespace traktor
{

/*! 4d vector.
 * \ingroup Core
 */
class T_MATH_ALIGN16 T_DLLCLASS Vector4
{
public:
#if defined(T_MATH_USE_SSE2)
	__m128 m_data;
#elif defined(T_MATH_USE_ALTIVEC) || defined(T_MATH_USE_ALTIVEC_SPU)
	vec_float4 m_data;
#elif defined(T_MATH_USE_NEON)
	float32x4_t m_data;
#else
	union
	{
		struct
		{
			float _x, _y, _z, _w;
		};

		float _e[4];
	};
#endif

	T_MATH_INLINE Vector4();

	T_MATH_INLINE Vector4(const Vector4& v);

#if defined(T_MATH_USE_SSE2)
	explicit T_MATH_INLINE Vector4(__m128 v);
#elif defined(T_MATH_USE_ALTIVEC) || defined(T_MATH_USE_ALTIVEC_SPU)
	explicit T_MATH_INLINE Vector4(vec_float4 v);
#elif defined(T_MATH_USE_NEON)
	explicit T_MATH_INLINE Vector4(float32x4_t v);
#endif

	explicit T_MATH_INLINE Vector4(const Scalar& s);

	explicit T_MATH_INLINE Vector4(float x, float y, float z, float w = 0);

	explicit T_MATH_INLINE Vector4(const float* p);

	static T_MATH_INLINE const Vector4& zero();

	static T_MATH_INLINE const Vector4& one();

	static T_MATH_INLINE const Vector4& origo();

	T_MATH_INLINE void set(float x, float y, float z, float w = 0);

	T_MATH_INLINE Scalar x() const;

	T_MATH_INLINE Scalar y() const;

	T_MATH_INLINE Scalar z() const;

	T_MATH_INLINE Scalar w() const;

	T_MATH_INLINE Vector4 xyz0() const;

	T_MATH_INLINE Vector4 xyz1() const;

	T_MATH_INLINE Vector4 _000w() const;

	template < int iX, int iY, int iZ, int iW >
	T_MATH_INLINE Vector4 shuffle() const
	{
#if defined(T_MATH_USE_SSE2)
		const int mask = iX | (iY << 2) | (iZ << 4) | (iW << 6);
		return Vector4(_mm_shuffle_ps(m_data, m_data, mask));
#elif defined(T_MATH_USE_ALTIVEC)
		const static uint32_t c_xyzw[] = { 0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f };
		Vector4 tmp;
		tmp.m_data = vec_perm(m_data, m_data, ((vec_uchar16)(vec_uint4){ c_xyzw[iX], c_xyzw[iY], c_xyzw[iZ], c_xyzw[iW] }));
		return tmp;
#else
		float T_MATH_ALIGN16 _e[4];
		storeAligned(_e);
		return Vector4(_e[iX], _e[iY], _e[iZ], _e[iW]);
#endif
	}

	T_MATH_INLINE Scalar length() const;

	T_MATH_INLINE Scalar length2() const;

	T_MATH_INLINE Scalar normalize();

	T_MATH_INLINE Vector4 normalized() const;

	T_MATH_INLINE Vector4 absolute() const;

	static T_MATH_INLINE Vector4 loadAligned(const float* in);

	static T_MATH_INLINE Vector4 loadUnaligned(const float* in);

	T_MATH_INLINE void storeAligned(float* out) const;

	T_MATH_INLINE void storeUnaligned(float* out) const;

	T_MATH_INLINE void storeUnaligned3(float* out) const;

	T_MATH_INLINE void storeIntegersAligned(int32_t* out) const;

	T_MATH_INLINE Scalar get(int index) const;

	T_MATH_INLINE void set(int index, const Scalar& value);

	T_MATH_INLINE Scalar min() const;

	T_MATH_INLINE Scalar max() const;

	T_MATH_INLINE Vector4 floor() const;

	T_MATH_INLINE Vector4& operator=(const Vector4& v);

	T_MATH_INLINE Vector4 operator-() const;

	T_MATH_INLINE Vector4& operator+=(const Scalar& v);

	T_MATH_INLINE Vector4& operator+=(const Vector4& v);

	T_MATH_INLINE Vector4& operator-=(const Scalar& v);

	T_MATH_INLINE Vector4& operator-=(const Vector4& v);

	T_MATH_INLINE Vector4& operator*=(const Scalar& v);

	T_MATH_INLINE Vector4& operator*=(const Vector4& v);

	T_MATH_INLINE Vector4& operator/=(const Scalar& v);

	T_MATH_INLINE Vector4& operator/=(const Vector4& v);

	T_MATH_INLINE bool operator==(const Vector4& v) const;

	T_MATH_INLINE bool operator!=(const Vector4& v) const;

	T_MATH_INLINE Scalar operator[](int index) const;

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator+(const Vector4& l, const Scalar& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator+(const Scalar& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator+(const Vector4& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator-(const Vector4& l, const Scalar& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator-(const Scalar& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator-(const Vector4& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator*(const Vector4& l, const Scalar& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator*(const Scalar& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator*(const Vector4& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator/(const Vector4& l, const Scalar& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator/(const Scalar& l, const Vector4& r);

	friend T_MATH_INLINE T_DLLCLASS Vector4 operator/(const Vector4& l, const Vector4& r);
};

T_MATH_INLINE T_DLLCLASS Scalar horizontalAdd3(const Vector4& v);

T_MATH_INLINE T_DLLCLASS Scalar horizontalAdd4(const Vector4& v);

T_MATH_INLINE T_DLLCLASS Scalar dot3(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS Scalar dot4(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS Vector4 cross(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS Vector4 lerp(const Vector4& a, const Vector4& b, const Scalar& c);

T_MATH_INLINE T_DLLCLASS Vector4 reflect(const Vector4& v, const Vector4& at);

T_MATH_INLINE T_DLLCLASS int32_t minorAxis3(const Vector4& v);

T_MATH_INLINE T_DLLCLASS int32_t majorAxis3(const Vector4& v);

T_MATH_INLINE T_DLLCLASS void orthogonalFrame(const Vector4& d, Vector4& outU, Vector4& outV);

T_MATH_INLINE T_DLLCLASS Vector4 min(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS Vector4 max(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS Vector4 clamp(const Vector4& value, const Vector4& minLimit, const Vector4& maxLimit);

T_MATH_INLINE T_DLLCLASS Vector4 select(const Vector4& condition, const Vector4& negative, const Vector4& positive);

T_MATH_INLINE T_DLLCLASS bool compareAllGreaterEqual(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS bool compareAllLessEqual(const Vector4& l, const Vector4& r);

T_MATH_INLINE T_DLLCLASS bool compareFuzzyEqual(const Vector4& l, const Vector4& r);

}

#if defined(T_MATH_USE_INLINE)
#	if defined(T_MATH_USE_SSE2)
#		include "Core/Math/Sse2/Vector4.inl"
#	elif defined(T_MATH_USE_ALTIVEC)
#		include "Core/Math/AltiVec/Vector4.inl"
#	elif defined(T_MATH_USE_NEON)
#		include "Core/Math/Neon/Vector4.inl"
#	else
#		include "Core/Math/Std/Vector4.inl"
#	endif
#endif
