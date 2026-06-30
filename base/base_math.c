#include "base_math.h"

////////////////////////
// F32 variant
internal Vec2F32 Vec2F32_Make(F32 x, F32 y)
{
	return (Vec2F32){x, y};
}

internal Vec3F32 Vec3F32_Make(F32 x, F32 y, F32 z)
{
	return (Vec3F32){x, y, z};
}

internal Vec4F32 Vec4F32_Make(F32 x, F32 y, F32 z, F32 w)
{
	return (Vec4F32){x, y, z, w};
}

internal Vec2F32 Vec2F32_Add(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	return result;
}

internal Vec3F32 Vec3F32_Add(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	return result;
}

internal Vec4F32 Vec4F32_Add(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	result.w	   = a.w + b.w;
	return result;
}

internal Vec2F32 Vec2F32_Sub(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	return result;
}

internal Vec3F32 Vec3F32_Sub(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	return result;
}

internal Vec4F32 Vec4F32_Sub(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	result.w	   = a.w - b.w;
	return result;
}

internal Vec2F32 Vec2F32_Mul(Vec2F32 a, Vec2F32 b)
{
	Vec2F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	return result;
}

internal Vec3F32 Vec3F32_Mul(Vec3F32 a, Vec3F32 b)
{
	Vec3F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	return result;
}

internal Vec4F32 Vec4F32_Mul(Vec4F32 a, Vec4F32 b)
{
	Vec4F32 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	result.w	   = a.w * b.w;
	return result;
}

internal F32 Vec2F32_Dot(Vec2F32 a, Vec2F32 b)
{
	return (a.x * b.x) + (a.y * b.y);
}

internal F32 Vec3F32_Dot(Vec3F32 a, Vec3F32 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

internal F32 Vec4F32_Dot(Vec4F32 a, Vec4F32 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

internal Vec2F32 Vec2F32_Scale(Vec2F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

internal Vec3F32 Vec3F32_Scale(Vec3F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

internal Vec4F32 Vec4F32_Scale(Vec4F32 a, F32 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;
	return a;
}

internal F32 Vec2F32_Length(Vec2F32 v)
{
	return sqrtf(Vec2F32_Dot(v, v));
}

internal F32 Vec3F32_Length(Vec3F32 v)
{
	return sqrtf(Vec3F32_Dot(v, v));
}

internal F32 Vec4F32_Length(Vec4F32 v)
{
	return sqrtf(Vec4F32_Dot(v, v));
}

internal F32 Vec2F32_LengthSquared(Vec2F32 v)
{
	return Vec2F32_Dot(v, v);
}
internal F32 Vec3F32_LengthSquared(Vec3F32 v)
{
	return Vec3F32_Dot(v, v);
}
internal F32 Vec4F32_LengthSquared(Vec4F32 v)
{
	return Vec4F32_Dot(v, v);
}

internal Vec2F32 Vec2F32_Normalize(Vec2F32 v)
{
	F32 len = Vec2F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec2F32_Make(0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec2F32_Scale(v, inverse_len);
}

internal Vec3F32 Vec3F32_Normalize(Vec3F32 v)
{
	F32 len = Vec3F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec3F32_Make(0, 0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec3F32_Scale(v, inverse_len);
}

internal Vec4F32 Vec4F32_Normalize(Vec4F32 v)
{
	F32 len = Vec4F32_Length(v);
	if (len < F32_EPSILON)
	{
		return Vec4F32_Make(0, 0, 0, 0);
	}
	F32 inverse_len = 1 / len;
	return Vec4F32_Scale(v, inverse_len);
}

////////////////////////
// F64 variant
internal Vec2F64 Vec2F64_Make(F64 x, F64 y)
{
	return (Vec2F64){x, y};
}
internal Vec3F64 Vec3F64_Make(F64 x, F64 y, F64 z)
{
	return (Vec3F64){x, y, z};
}
internal Vec4F64 Vec4F64_Make(F64 x, F64 y, F64 z, F64 w)
{
	return (Vec4F64){x, y, z, w};
}

internal Vec2F64 Vec2F64_Add(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	return result;
}
internal Vec3F64 Vec3F64_Add(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	return result;
}
internal Vec4F64 Vec4F64_Add(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x + b.x;
	result.y	   = a.y + b.y;
	result.z	   = a.z + b.z;
	result.w	   = a.w + b.w;
	return result;
}

internal Vec2F64 Vec2F64_Sub(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	return result;
}
internal Vec3F64 Vec3F64_Sub(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	return result;
}
internal Vec4F64 Vec4F64_Sub(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x - b.x;
	result.y	   = a.y - b.y;
	result.z	   = a.z - b.z;
	result.w	   = a.w - b.w;
	return result;
}

internal Vec2F64 Vec2F64_Mul(Vec2F64 a, Vec2F64 b)
{
	Vec2F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	return result;
}
internal Vec3F64 Vec3F64_Mul(Vec3F64 a, Vec3F64 b)
{
	Vec3F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	return result;
}
internal Vec4F64 Vec4F64_Mul(Vec4F64 a, Vec4F64 b)
{
	Vec4F64 result = {0};
	result.x	   = a.x * b.x;
	result.y	   = a.y * b.y;
	result.z	   = a.z * b.z;
	result.w	   = a.w * b.w;
	return result;
}

internal F64 Vec2F64_Dot(Vec2F64 a, Vec2F64 b)
{
	return (a.x * b.x) + (a.y * b.y);
}
internal F64 Vec3F64_Dot(Vec3F64 a, Vec3F64 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
internal F64 Vec4F64_Dot(Vec4F64 a, Vec4F64 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

internal Vec2F64 Vec2F64_Scale(Vec2F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	return a;
}
internal Vec3F64 Vec3F64_Scale(Vec3F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}
internal Vec4F64 Vec4F64_Scale(Vec4F64 a, F64 b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;
	return a;
}

internal F64 Vec2F64_Length(Vec2F64 v)
{
	return sqrt(Vec2F64_Dot(v, v));
}
internal F64 Vec3F64_Length(Vec3F64 v)
{
	return sqrt(Vec3F64_Dot(v, v));
}
internal F64 Vec4F64_Length(Vec4F64 v)
{
	return sqrt(Vec4F64_Dot(v, v));
}

internal F64 Vec2F64_LengthSquared(Vec2F64 v)
{
	return Vec2F64_Dot(v, v);
}
internal F64 Vec3F64_LengthSquared(Vec3F64 v)
{
	return Vec3F64_Dot(v, v);
}
internal F64 Vec4F64_LengthSquared(Vec4F64 v)
{
	return Vec4F64_Dot(v, v);
}

internal Vec2F64 Vec2F64_Normalize(Vec2F64 v)
{
	F64 len = Vec2F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec2F64_Make(0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec2F64_Scale(v, inverse_len);
}

internal Vec3F64 Vec3F64_Normalize(Vec3F64 v)
{
	F64 len = Vec3F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec3F64_Make(0, 0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec3F64_Scale(v, inverse_len);
}

internal Vec4F64 Vec4F64_Normalize(Vec4F64 v)
{
	F64 len = Vec4F64_Length(v);
	if (len < F64_EPSILON)
	{
		return Vec4F64_Make(0, 0, 0, 0);
	}
	F64 inverse_len = 1.0 / len;
	return Vec4F64_Scale(v, inverse_len);
}

internal Mat4x4F32 Mat4x4F32_MakeFromVec3Columns(Vec3F32 c0, Vec3F32 c1, Vec3F32 c2, Vec3F32 c3)
{
	// c0, c1, c2 = rotation/scale basis vectors (X, Y, Z axes)
	// c3         = translation
	// 4th component (w) filled in implicitly: 0 for basis columns, 1 for translation
	Mat4x4F32 result;
	result.columns[0] = Vec4F32_Make(c0.x, c0.y, c0.z, 0.0f);
	result.columns[1] = Vec4F32_Make(c1.x, c1.y, c1.z, 0.0f);
	result.columns[2] = Vec4F32_Make(c2.x, c2.y, c2.z, 0.0f);
	result.columns[3] = Vec4F32_Make(c3.x, c3.y, c3.z, 1.0f);
	return result;
}

// Must be removed probably ...
internal Mat4x4F32 Mat4x4F32_MakeFromVec3Rows(Vec3F32 r0, Vec3F32 r1, Vec3F32 r2, Vec3F32 r3)
{
	// r0, r1, r2, r3 are logical ROWS of the matrix (3 components each,
	// 4th component implicitly 0 except r3 which is implicitly an affine row -> w = 1 is NOT assumed here,
	// since rows don't carry the same "basis + translation" meaning columns do.
	// We pad every row's 4th component with 0 to keep this purely structural, not affine-specific.
	//
	// Storage is column-major, so we have to transpose: each row's components get
	// scattered across the four columns at the matching row-index.
	Mat4x4F32 result;
	result.columns[0] = Vec4F32_Make(r0.x, r1.x, r2.x, r3.x);
	result.columns[1] = Vec4F32_Make(r0.y, r1.y, r2.y, r3.y);
	result.columns[2] = Vec4F32_Make(r0.z, r1.z, r2.z, r3.z);
	result.columns[3] = Vec4F32_Make(0.0f, 0.0f, 0.0f, 0.0f);
	return result;
}
